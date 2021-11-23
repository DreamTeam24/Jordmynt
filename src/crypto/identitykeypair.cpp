#include "identitykeypair.h"

#include "logger/logger.h"
#include <openssl/pem.h>
#include <utility>

namespace jm {
namespace crypto {

Bytes::size_type IdentityKeyPair::getPublicIdentityKeySize()
{
    return s_keySize;
}

IdentityKeyPair::IdentityKeyPair() :
    m_keyPair{ nullptr },
    m_keyContext{ EVP_PKEY_CTX_new_id(s_id, nullptr) },
    m_hashContext{ EVP_MD_CTX_new() }
{
    logger::DEBUG("Creating a IdentityKeyPair ...");

    if (m_keyContext == nullptr)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW("OpenSSL: EVP_PKEY_CTX_new_id failed");
    }

    if (EVP_PKEY_keygen_init(m_keyContext) != 1)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW("OpenSSL: EVP_PKEY_keygen_init failed");
    }

    if (EVP_PKEY_keygen(m_keyContext, &m_keyPair) != 1)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW("OpenSSL: EVP_PKEY_keygen failed");
    }

    initialize();
}

IdentityKeyPair::IdentityKeyPair(Bytes const& privateIdentityKey) :
    m_keyPair{ EVP_PKEY_new_raw_private_key(s_id, nullptr, &privateIdentityKey[0], privateIdentityKey.size()) },
    m_keyContext{ EVP_PKEY_CTX_new(m_keyPair, nullptr) },
    m_hashContext{ EVP_MD_CTX_new() }
{
    logger::DEBUG("Creating a IdentityKeyPair from a private key ...");
            
    if (m_keyPair == nullptr)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW("OpenSSL: EVP_PKEY_new_raw_private_key failed");
    }

    if (m_keyContext == nullptr)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW("OpenSSL: EVP_PKEY_CTX_new failed");
    }

    initialize();
}

IdentityKeyPair::IdentityKeyPair(std::string const& privateIdentityKeyFilename) :
    m_keyPair{ nullptr },
    m_keyContext{ nullptr },
    m_hashContext{ EVP_MD_CTX_new() }
{
    logger::DEBUG("Creating a IdentityKeyPair from a private key file ...");

    BIO* privateKeyFile{ BIO_new_file(privateIdentityKeyFilename.c_str(), "r") };
    if (privateKeyFile == nullptr)
        logger::THROW("OpenSSL: BIO_new_file failed");

    m_keyPair = PEM_read_bio_PrivateKey(privateKeyFile, nullptr, nullptr, nullptr);
    BIO_free(privateKeyFile);
    if (m_keyPair == nullptr)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW("OpenSSL: PEM_read_bio_PrivateKey failed");
    }

    if (EVP_PKEY_id(m_keyPair) != s_id)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW(privateIdentityKeyFilename + " contains an invalid private key type.");
    }

    m_keyContext = EVP_PKEY_CTX_new(m_keyPair, nullptr);
    if (m_keyContext == nullptr)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW("OpenSSL: EVP_PKEY_CTX_new failed");
    }

    initialize();
}

void IdentityKeyPair::initialize()
{
    if (EVP_DigestSignInit(m_hashContext, nullptr, nullptr, nullptr, m_keyPair) != 1)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW("OpenSSL: EVP_DigestSignInit failed");
    }
}

IdentityKeyPair::~IdentityKeyPair()
{
    free();
}

void IdentityKeyPair::free()
{
    EVP_MD_CTX_free(m_hashContext);
    EVP_PKEY_CTX_free(m_keyContext);
    EVP_PKEY_free(m_keyPair);
}

IdentityKeyPair::IdentityKeyPair(IdentityKeyPair&& identityKeyPair) noexcept :
    m_keyPair{ std::exchange(identityKeyPair.m_keyPair, nullptr) },
    m_keyContext{ std::exchange(identityKeyPair.m_keyContext, nullptr) },
    m_hashContext{ std::exchange(identityKeyPair.m_hashContext, nullptr) }
{
}

IdentityKeyPair& IdentityKeyPair::operator=(IdentityKeyPair&& identityKeyPair) noexcept
{
    m_keyPair = std::exchange(identityKeyPair.m_keyPair, nullptr);
    m_keyContext = std::exchange(identityKeyPair.m_keyContext, nullptr);
    m_hashContext = std::exchange(identityKeyPair.m_hashContext, nullptr);
    return *this;
}

Bytes IdentityKeyPair::getPublicIdentityKey() const
{
    logger::DEBUG("Getting a public identity key ...");
    
    Bytes::size_type publicIdentityKeySize{ s_keySize };    // OpenSSL needs this
    Bytes publicIdentityKey(publicIdentityKeySize);
    if (EVP_PKEY_get_raw_public_key(m_keyPair, &publicIdentityKey[0], &publicIdentityKeySize) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_get_raw_public_key failed");

    return publicIdentityKey;
}

Bytes IdentityKeyPair::sign(Bytes const& message) const
{
    logger::DEBUG("Signing a message ...");

    Bytes::size_type signatureSize{ s_signatureSize };      // OpenSSL needs this
    Bytes signature(signatureSize);
    if (EVP_DigestSign(m_hashContext, &signature[0], &signatureSize, (message.empty() ? nullptr : &message[0]), message.size()) != 1)
        logger::THROW("OpenSSL: EVP_DigestSign failed");

    return signature;
}

} // namespace crypto
} // namespace jm
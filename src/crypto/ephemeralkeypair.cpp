#include "ephemeralkeypair.h"

#include "logger/logger.h"
#include <openssl/pem.h>
#include <cassert>
#include <utility>

namespace jm {
namespace crypto {

Bytes::size_type EphemeralKeyPair::getPublicEphemeralKeySize()
{
    return s_keySize;
}

EphemeralKeyPair::EphemeralKeyPair() :
    m_keyPair{ nullptr },
    m_keyContext{ EVP_PKEY_CTX_new_id(s_id, nullptr) }
{
    logger::DEBUG("Creating an EphemeralKeyPair ...");

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

EphemeralKeyPair::EphemeralKeyPair(Bytes const& privateKey) :
    m_keyPair{ EVP_PKEY_new_raw_private_key(s_id, nullptr, &privateKey[0], privateKey.size()) },
    m_keyContext{ EVP_PKEY_CTX_new(m_keyPair, nullptr) }
{
    logger::DEBUG("Creating an EphemeralKeyPair from a private key ...");
            
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

EphemeralKeyPair::EphemeralKeyPair(std::string const& privateKeyFilename) :
    m_keyPair{ nullptr },
    m_keyContext{ nullptr }
{
    logger::DEBUG("Creating an EphemeralKeyPair from a private key file ...");

    BIO* privateKeyFile{ BIO_new_file(privateKeyFilename.c_str(), "r") };
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
        logger::THROW(privateKeyFilename + " contains an invalid private key type.");
    }

    m_keyContext = EVP_PKEY_CTX_new(m_keyPair, nullptr);
    if (m_keyContext == nullptr)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW("OpenSSL: EVP_PKEY_CTX_new failed");
    }

    initialize();
}

void EphemeralKeyPair::initialize()
{
    if (EVP_PKEY_derive_init(m_keyContext) != 1)
    {
        free(); // prevent memory leak on constructor throw
        logger::THROW("OpenSSL: EVP_PKEY_derive_init failed");
    }
}

EphemeralKeyPair::~EphemeralKeyPair()
{
    free();
}

void EphemeralKeyPair::free()
{
    EVP_PKEY_CTX_free(m_keyContext);
    EVP_PKEY_free(m_keyPair);
}

EphemeralKeyPair::EphemeralKeyPair(EphemeralKeyPair&& ephemeralKeyPair) noexcept :
    m_keyPair{ std::exchange(ephemeralKeyPair.m_keyPair, nullptr) },
    m_keyContext{ std::exchange(ephemeralKeyPair.m_keyContext, nullptr) }
{
}

EphemeralKeyPair& EphemeralKeyPair::operator=(EphemeralKeyPair&& ephemeralKeyPair) noexcept
{
    m_keyPair = std::exchange(ephemeralKeyPair.m_keyPair, nullptr);
    m_keyContext = std::exchange(ephemeralKeyPair.m_keyContext, nullptr);
    return *this;
}

Bytes EphemeralKeyPair::getPublicEphemeralKey() const
{
    logger::DEBUG("Getting a public ephemeral key ...");
    
    Bytes::size_type publicEphemeralKeySize{ s_keySize };   // OpenSSL needs this
    Bytes publicEphemeralKey(publicEphemeralKeySize);
    if (EVP_PKEY_get_raw_public_key(m_keyPair, &publicEphemeralKey[0], &publicEphemeralKeySize) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_get_raw_public_key failed");

    return publicEphemeralKey;
}

Bytes EphemeralKeyPair::deriveSharedSecret(Bytes const& peerPublicEphemeralKey) const
{
    assert(peerPublicEphemeralKey.size() == s_keySize);

    logger::DEBUG("Deriving a shared secret ...");
    
    EVP_PKEY* peerEphemeralKeyPair{ EVP_PKEY_new_raw_public_key(s_id, nullptr, &peerPublicEphemeralKey[0], peerPublicEphemeralKey.size()) };
    if (peerEphemeralKeyPair == nullptr)
        logger::THROW("OpenSSL: EVP_PKEY_new_raw_public_key failed");

    if (EVP_PKEY_derive_set_peer(m_keyContext, peerEphemeralKeyPair) != 1)
    {
        EVP_PKEY_free(peerEphemeralKeyPair);        // prevent memory leak on throw
        logger::THROW("OpenSSL: EVP_PKEY_derive_set_peer failed");
    }

    Bytes::size_type sharedSecretSize{ s_keySize }; // OpenSSL needs this
    Bytes sharedSecret(sharedSecretSize);
    if (EVP_PKEY_derive(m_keyContext, &sharedSecret[0], &sharedSecretSize) != 1)
    {
        EVP_PKEY_free(peerEphemeralKeyPair);        // prevent memory leak on throw
        logger::THROW("OpenSSL: EVP_PKEY_derive failed");
    }
    
    EVP_PKEY_free(peerEphemeralKeyPair);

    return sharedSecret;
}

} // namespace crypto
} // namespace jm
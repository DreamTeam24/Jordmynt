#include "identitykey.h"
#include "opensslerror.h"
#include "logger/logger.h"
#include <openssl/core_names.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <utility>
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
std::string const IdentityKey::s_algorithm{ SN_ED25519 };
// ----------------------------------------------------------------------------
BytesSize const   IdentityKey::s_publicKeySize{ 32 };
// ----------------------------------------------------------------------------
BytesSize const   IdentityKey::s_signatureSize{ 64 };
// ----------------------------------------------------------------------------
IdentityKey::IdentityKey() :
    m_key{ nullptr },
    m_keyContext{ EVP_PKEY_CTX_new_from_name(nullptr,
                                             s_algorithm.c_str(),
                                             nullptr) },
    m_signContext{ EVP_MD_CTX_new() }
{
    logger::Debug("Creating an IdentityKey ...");

    if (!m_keyContext)
        throw OpenSSLError{};

    if (EVP_PKEY_keygen_init(m_keyContext.get()) != 1)
        throw OpenSSLError{};

    EVP_PKEY* identityKey{ nullptr }; 
    if (EVP_PKEY_generate(m_keyContext.get(),
                          &identityKey) != 1)
        throw OpenSSLError{};

    m_key.reset(identityKey);

    if (!m_signContext)
        throw OpenSSLError{};
}
// ----------------------------------------------------------------------------
IdentityKey::IdentityKey(BytesView privateKey) :
    m_key{ EVP_PKEY_new_raw_private_key_ex(nullptr,
                                           s_algorithm.c_str(),
                                           nullptr,
                                           privateKey.data(),
                                           privateKey.size()) },
    m_keyContext{ EVP_PKEY_CTX_new_from_pkey(nullptr,
                                            m_key.get(),
                                            nullptr) },
    m_signContext{ EVP_MD_CTX_new() }
{
    logger::Debug("Creating a IdentityKey from a private key ...");
            
    if (!m_key)
        throw OpenSSLError{};

    if (!m_keyContext)
        throw OpenSSLError{};

    if (!m_signContext)
        throw OpenSSLError{};
}
// ----------------------------------------------------------------------------
IdentityKey::IdentityKey(std::string_view privateKeyFilename) :
    m_key{ nullptr },
    m_keyContext{ nullptr },
    m_signContext{ EVP_MD_CTX_new() }
{
    logger::Debug("Creating a IdentityKey from a private key file ...");

    Pointer<BIO> privateKeyFile{ BIO_new_file(privateKeyFilename.data(),
                                              "r") };
    if (!privateKeyFile)
        throw OpenSSLError{};

    m_key.reset(PEM_read_bio_PrivateKey(privateKeyFile.get(),
                                        nullptr,
                                        nullptr,
                                        nullptr));
    if (!m_key)
        throw OpenSSLError{};

    char algorithm[64]{};
    if (EVP_PKEY_get_utf8_string_param(m_key.get(),
                                       OSSL_PKEY_PARAM_GROUP_NAME,
                                       algorithm,
                                       64,
                                       nullptr) != 1)
        throw OpenSSLError{};

    if (std::string(algorithm) != s_algorithm)
        throw std::runtime_error{ "Invalid ephemeral key type" };

    m_keyContext.reset(EVP_PKEY_CTX_new_from_pkey(nullptr,
                                                  m_key.get(),
                                                  nullptr));
    if (!m_keyContext)
        throw OpenSSLError{};

    if (!m_signContext)
        throw OpenSSLError{};
}
// ----------------------------------------------------------------------------
IdentityKey::~IdentityKey()
{
}
// ----------------------------------------------------------------------------
IdentityKey::IdentityKey(IdentityKey&& identityKey) noexcept :
    m_key{ std::move(identityKey.m_key) },
    m_keyContext{ std::move(identityKey.m_keyContext) },
    m_signContext{ std::move(identityKey.m_signContext) }
{
}
// ----------------------------------------------------------------------------
IdentityKey& IdentityKey::operator=(IdentityKey&& identityKey) noexcept
{
    m_key = std::move(identityKey.m_key);
    m_keyContext = std::move(identityKey.m_keyContext);
    m_signContext = std::move(identityKey.m_signContext);
    return *this;
}
// ----------------------------------------------------------------------------
BytesSize IdentityKey::getPublicKeySize()
{
    return s_publicKeySize;
}
// ----------------------------------------------------------------------------
BytesSpan IdentityKey::getPublicKey(BytesSpan& buffer) const
{
    logger::Debug("Getting a public identity key ...");

    BytesSpan publicKey{ defineVariable(buffer, s_publicKeySize) };
    BytesSize publicKeySize{ s_publicKeySize };
    if (EVP_PKEY_get_raw_public_key(m_key.get(),
                                    publicKey.data(),
                                    &publicKeySize) != 1)
        throw OpenSSLError{};

    return publicKey;
}
// ----------------------------------------------------------------------------
BytesSize IdentityKey::getSignatureSize()
{
    return s_signatureSize;
}
// ----------------------------------------------------------------------------
BytesSpan IdentityKey::sign(BytesView  message,
                            BytesSpan& buffer) const
{
    logger::Debug("Signing a message ...");

    if (EVP_DigestSignInit_ex(m_signContext.get(),
                              nullptr,
                              nullptr,
                              nullptr,
                              nullptr,
                              m_key.get(),
                              nullptr) != 1)
        throw OpenSSLError{};

    BytesSpan signature{ defineVariable(buffer, s_signatureSize) };
    BytesSize signatureSize{ s_signatureSize };
    if (EVP_DigestSign(m_signContext.get(),
                       signature.data(),
                       &signatureSize,
                       message.data(),
                       message.size()) != 1)
        throw OpenSSLError{};

    return signature;
}
// ----------------------------------------------------------------------------
bool IdentityKey::verify(BytesView message,
                         BytesView signature,
                         BytesView peerPublicKey)
{
    logger::Debug("Verifying a signature ...");

    Pointer<EVP_PKEY> peerKey{
        EVP_PKEY_new_raw_public_key_ex(nullptr,
                                       s_algorithm.c_str(),
                                       nullptr,
                                       peerPublicKey.data(),
                                       peerPublicKey.size()) };
    if (!peerKey)
        throw OpenSSLError{};

    Pointer<EVP_MD_CTX> verifyContext{ EVP_MD_CTX_new() };
    if (!verifyContext)
        throw OpenSSLError{};

    if (EVP_DigestVerifyInit_ex(verifyContext.get(),
                                nullptr,
                                nullptr,
                                nullptr,
                                nullptr,
                                peerKey.get(),
                                nullptr) != 1)
        throw OpenSSLError{};

    return (EVP_DigestVerify(verifyContext.get(),
                             signature.data(),
                             signature.size(),
                             message.data(),
                             message.size()) == 1);
}
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
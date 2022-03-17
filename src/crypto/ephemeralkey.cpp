#include "ephemeralkey.h"
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
std::string const EphemeralKey::s_algorithm{ SN_X25519 };
// ----------------------------------------------------------------------------
BytesSize const   EphemeralKey::s_publicKeySize{ 32 };
// ----------------------------------------------------------------------------
BytesSize const   EphemeralKey::s_sharedSecretSize{ 32 };
// ----------------------------------------------------------------------------
EphemeralKey::EphemeralKey() :
    m_key{ nullptr },
    m_keyContext{ EVP_PKEY_CTX_new_from_name(nullptr,
                                             s_algorithm.c_str(),
                                             nullptr) }
{
    logger::Debug("Creating an EphemeralKey ...");

    if (!m_keyContext)
        throw OpenSSLError{};

    if (EVP_PKEY_keygen_init(m_keyContext.get()) != 1)
        throw OpenSSLError{};

    EVP_PKEY* key{ nullptr };
    if (EVP_PKEY_generate(m_keyContext.get(), 
                          &key) != 1)
        throw OpenSSLError{};
    
    m_key.reset(key);
}
// ----------------------------------------------------------------------------
EphemeralKey::EphemeralKey(BytesView privateKey) :
    m_key{ EVP_PKEY_new_raw_private_key_ex(nullptr,
                                           s_algorithm.c_str(),
                                           nullptr,
                                           privateKey.data(),
                                           privateKey.size()) },
    m_keyContext{ EVP_PKEY_CTX_new_from_pkey(nullptr,
                                             m_key.get(),
                                             nullptr) }
{
    logger::Debug("Creating an EphemeralKey from a private key ...");
            
    if (!m_key)
        throw OpenSSLError{};

    if (!m_keyContext)
        throw OpenSSLError{};
}
// ----------------------------------------------------------------------------
EphemeralKey::EphemeralKey(std::string_view privateKeyFilename) :
    m_key{ nullptr },
    m_keyContext{ nullptr }
{
    logger::Debug("Creating an EphemeralKey from a private key file ...");

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
}
// ----------------------------------------------------------------------------
EphemeralKey::~EphemeralKey()
{
}
// ----------------------------------------------------------------------------
EphemeralKey::EphemeralKey(EphemeralKey&& ephemeralKey) noexcept :
    m_key{ std::move(ephemeralKey.m_key) },
    m_keyContext{ std::move(ephemeralKey.m_keyContext) }
{
}
// ----------------------------------------------------------------------------
EphemeralKey& EphemeralKey::operator=(EphemeralKey&& ephemeralKey) noexcept
{
    m_key = std::move(ephemeralKey.m_key);
    m_keyContext = std::move(ephemeralKey.m_keyContext);
    return *this;
}
// ----------------------------------------------------------------------------
BytesSize EphemeralKey::getPublicKeySize()
{
    return s_publicKeySize;
}
// ----------------------------------------------------------------------------
BytesSpan EphemeralKey::getPublicKey(BytesSpan& buffer) const
{
    logger::Debug("Getting a public ephemeral key ...");

    BytesSpan publicKey{ defineVariable(buffer, s_publicKeySize) };
    BytesSize publicKeySize{ s_publicKeySize };
    if (EVP_PKEY_get_raw_public_key(m_key.get(),
                                    publicKey.data(),
                                    &publicKeySize) != 1)
        throw OpenSSLError{};

    return publicKey;
}
// ----------------------------------------------------------------------------
BytesSize EphemeralKey::getSharedSecretSize()
{
    return s_sharedSecretSize;
}
// ----------------------------------------------------------------------------
BytesSpan EphemeralKey::deriveSharedSecret(BytesView  peerPublicKey,
                                           BytesSpan& buffer) const
{
    logger::Debug("Deriving a shared secret ...");
    
    if (EVP_PKEY_derive_init_ex(m_keyContext.get(),
                                nullptr) != 1)
        throw OpenSSLError{};
    
    Pointer<EVP_PKEY> peerKey{
        EVP_PKEY_new_raw_public_key_ex(nullptr,
                                       s_algorithm.c_str(),
                                       nullptr,
                                       peerPublicKey.data(),
                                       peerPublicKey.size()) };
    if (!peerKey)
        throw OpenSSLError{};

    if (EVP_PKEY_derive_set_peer_ex(m_keyContext.get(),
                                    peerKey.get(),
                                    1) != 1)
        throw OpenSSLError{};

    BytesSpan sharedSecret{ defineVariable(buffer, s_sharedSecretSize) };
    BytesSize sharedSecretSize{ s_sharedSecretSize };
    if (EVP_PKEY_derive(m_keyContext.get(),
                        sharedSecret.data(),
                        &sharedSecretSize) != 1)
        throw OpenSSLError{};

    return sharedSecret;
}
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
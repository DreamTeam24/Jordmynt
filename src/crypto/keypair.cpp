#include "keypair.h"
#include "logger/logger.h"
#include <openssl/pem.h>
#include <cassert>

namespace jm {
namespace crypto {

KeyPair::KeyPair() :
    m_keyPair { nullptr }
{
    logger::DEBUG("Creating a KeyPair ...");

    EVP_PKEY_CTX* keyContext{ EVP_PKEY_CTX_new_id(s_id, nullptr) };
    if (keyContext == nullptr)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_new_id failed");

    if (EVP_PKEY_keygen_init(keyContext) != 1)
    {
        EVP_PKEY_CTX_free(keyContext);  // prevent memory leak on throw
        logger::THROW("OpenSSL: EVP_PKEY_keygen_init failed");
    }

    if (EVP_PKEY_keygen(keyContext, &m_keyPair) != 1)
    {
        EVP_PKEY_CTX_free(keyContext);  // prevent memory leak on throw
        logger::THROW("OpenSSL: EVP_PKEY_keygen failed");
    }

    EVP_PKEY_CTX_free(keyContext);
}

KeyPair::KeyPair(Bytes const& privateKey) :
    m_keyPair{ nullptr }
{
    assert(privateKey.size() == s_keySize);

    logger::DEBUG("Creating a KeyPair from a private key ...");
            
    m_keyPair = EVP_PKEY_new_raw_private_key(s_id, nullptr, &privateKey[0], privateKey.size());
    if (m_keyPair == nullptr)
        logger::THROW("OpenSSL: EVP_PKEY_new_raw_private_key failed");
}

KeyPair::KeyPair(std::string const& privateKeyFilename) :
    m_keyPair{ nullptr }
{
    logger::DEBUG("Creating a KeyPair from a private key file ...");

    BIO* privateKeyFile{ BIO_new_file(privateKeyFilename.c_str(), "r") };
    if (privateKeyFile == nullptr)
        logger::THROW("OpenSSL: BIO_new_file failed");

    m_keyPair = PEM_read_bio_PrivateKey(privateKeyFile, nullptr, nullptr, nullptr);
    BIO_free(privateKeyFile);
    if (m_keyPair == nullptr)
        logger::THROW("OpenSSL: PEM_read_bio_PrivateKey failed");

    if (EVP_PKEY_id(m_keyPair) != s_id)
        logger::THROW(privateKeyFilename + " contains an invalid private key type.");
}

KeyPair::~KeyPair()
{
    EVP_PKEY_free(m_keyPair);
}

Bytes::size_type KeyPair::getPublicKeySize()
{
    return s_keySize;
}

Bytes KeyPair::getPublicKey() const
{
    logger::DEBUG("Getting a public key ...");
    
    Bytes::size_type publicKeySize{ s_keySize };        // OpenSSL needs this
    Bytes publicKey(publicKeySize);                     // {}-initialization does not set the size
    
    if (EVP_PKEY_get_raw_public_key(m_keyPair, &publicKey[0], &publicKeySize) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_get_raw_public_key failed");

    return publicKey;
}

Bytes KeyPair::deriveSharedSecret(Bytes const& peerPublicKey) const
{
    assert(peerPublicKey.size() == s_keySize);
    logger::DEBUG("Deriving a shared secret ...");

    EVP_PKEY_CTX* keyContext{ EVP_PKEY_CTX_new(m_keyPair, nullptr) };
    if (keyContext == nullptr)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_new_id failed");
    
    EVP_PKEY* peerKeyPair{ EVP_PKEY_new_raw_public_key(s_id, nullptr, &peerPublicKey[0], peerPublicKey.size()) };
    if (peerKeyPair == nullptr)
    {
        EVP_PKEY_CTX_free(keyContext);  // prevent memory leak on throw
        logger::THROW("OpenSSL: EVP_PKEY_new_raw_public_key failed");
    }

    if (EVP_PKEY_derive_init(keyContext) != 1)
    {
        EVP_PKEY_free(peerKeyPair);     // prevent memory leak on throw
        EVP_PKEY_CTX_free(keyContext);  // prevent memory leak on throw
        logger::THROW("OpenSSL: EVP_PKEY_derive_init failed");
    }

    if (EVP_PKEY_derive_set_peer(keyContext, peerKeyPair) != 1)
    {
        EVP_PKEY_free(peerKeyPair);     // prevent memory leak on throw
        EVP_PKEY_CTX_free(keyContext);  // prevent memory leak on throw
        logger::THROW("OpenSSL: EVP_PKEY_derive_set_peer failed");
    }

    Bytes::size_type sharedSecretSize{ s_keySize };     // OpenSSL needs this
    Bytes sharedSecret(sharedSecretSize);               // {}-initialization does not set the size
    if (EVP_PKEY_derive(keyContext, &sharedSecret[0], &sharedSecretSize) != 1)
    {
        EVP_PKEY_free(peerKeyPair);     // prevent memory leak on throw
        EVP_PKEY_CTX_free(keyContext);  // prevent memory leak on throw
        logger::THROW("OpenSSL: EVP_PKEY_derive failed");
    }
    
    EVP_PKEY_free(peerKeyPair); 
    EVP_PKEY_CTX_free(keyContext);

    return sharedSecret;
}

} // namespace crypto
} // namespace jm
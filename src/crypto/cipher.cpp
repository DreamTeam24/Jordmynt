#include "cipher.h"
#include "logger/logger.h"
#include <cassert>
#include <limits>

namespace jm {
namespace crypto {

Cipher::Cipher(Bytes key,
               Bytes initializationVector) :
    m_cipherContext{ nullptr },
    m_key{ key },
    m_initializationVector{ initializationVector },
    m_encryptionCounter{ 0 },
    m_decryptionCounter{ 0 }
{
    assert(key.size() == s_keySize);
    assert(initializationVector.size() == s_initializationVectorSize);

    logger::DEBUG("Creating a Cipher ...");

    m_cipherContext = EVP_CIPHER_CTX_new();
    if (m_cipherContext == nullptr)
        logger::THROW("OpenSSL: EVP_CIPHER_CTX_new failed");
}

Cipher::~Cipher()
{
    EVP_CIPHER_CTX_free(m_cipherContext);
}

Bytes Cipher::encrypt(Bytes const& plaintext,
                      Bytes const& additionalAuthenticatedData,
                      Bytes&       authenticationTag)
{
    assert(plaintext.size() <= std::numeric_limits<int>::max());

    logger::DEBUG("Encrypting plaintext ...");

    // XOR last 8 bytes of initialization vector with encryption counter
    Bytes sequenceInitializationVector{ m_initializationVector };
    for (Bytes::size_type i = 0; i < 8; i++)
        sequenceInitializationVector[s_initializationVectorSize - 1 - i] ^= ((m_encryptionCounter >> (i * 8)) & 0xFF);

    if (EVP_EncryptInit_ex(m_cipherContext, s_cipher, nullptr, &(m_key[0]), &(sequenceInitializationVector[0])) != 1)
        logger::THROW("OpenSSL: EVP_EncryptInit_ex failed");
    
    int nBytes(0);          // OpenSSL needs this
    if (EVP_EncryptUpdate(m_cipherContext, nullptr, &nBytes, &additionalAuthenticatedData[0], static_cast<int>(additionalAuthenticatedData.size())) != 1)
        logger::THROW("OpenSSL: EVP_EncryptUpdate failed");
    
    Bytes ciphertext(plaintext.size());     // {}-initialization does not set the size
    if (EVP_EncryptUpdate(m_cipherContext, &ciphertext[0], &nBytes, &plaintext[0], static_cast<int>(plaintext.size())) != 1)
        logger::THROW("OpenSSL: EVP_EncryptUpdate failed");
    
    Bytes dummy{ 0 };       // OpenSSL needs this
    if (EVP_EncryptFinal_ex(m_cipherContext, &dummy[0], &nBytes) != 1)  //does nothing in GCM mode
        logger::THROW("OpenSSL: EVP_EncryptFinal_ex failed");
    
    authenticationTag.resize(s_authenticationTagSize);
    if (EVP_CIPHER_CTX_ctrl(m_cipherContext, EVP_CTRL_GCM_GET_TAG, static_cast<int>(s_authenticationTagSize), &authenticationTag[0]) != 1)
        logger::THROW("OpenSSL: EVP_CIPHER_CTX_ctrl failed");

    ++m_encryptionCounter;  // increment counter for next encryption

    return ciphertext;      // authenticationTag is returned as a parameter
}

Bytes Cipher::decrypt(Bytes const& ciphertext,
                      Bytes const& additionalAuthenticatedData,
                      Bytes&       authenticationTag)
{
    assert(ciphertext.size() <= std::numeric_limits<int>::max());

    logger::DEBUG("Decrypting plaintext ...");

    // XOR last 8 bytes of initialization vector with decryption counter
    Bytes sequenceInitializationVector{ m_initializationVector };
    for (Bytes::size_type i = 0; i < 8; i++)
        sequenceInitializationVector[s_initializationVectorSize - 1 - i] ^= ((m_decryptionCounter >> (i * 8)) & 0xFF);

    if (EVP_DecryptInit_ex(m_cipherContext, s_cipher, nullptr, &(m_key[0]), &(sequenceInitializationVector[0])) != 1)
        logger::THROW("OpenSSL: EVP_DecryptInit_ex failed");

    int nBytes(0);          // OpenSSL needs this
    if (EVP_DecryptUpdate(m_cipherContext, nullptr, &nBytes, &additionalAuthenticatedData[0], static_cast<int>(additionalAuthenticatedData.size())) != 1)
        logger::THROW("OpenSSL: EVP_DecryptUpdate failed");

    Bytes plaintext(ciphertext.size());     // {}-initialization does not set the size
    if (EVP_DecryptUpdate(m_cipherContext, &plaintext[0], &nBytes, &ciphertext[0], static_cast<int>(ciphertext.size())) != 1)
        logger::THROW("OpenSSL: EVP_DecryptUpdate failed");
    
    authenticationTag.resize(s_authenticationTagSize);
    if (EVP_CIPHER_CTX_ctrl(m_cipherContext, EVP_CTRL_GCM_SET_TAG, static_cast<int>(s_authenticationTagSize), &authenticationTag[0]) != 1)
        logger::THROW("OpenSSL: EVP_CIPHER_CTX_ctrl failed");

    Bytes dummy{ 0 };       // OpenSSL needs this
    if (EVP_DecryptFinal_ex(m_cipherContext, &dummy[0], &nBytes) != 1)  //does nothing in GCM mode
        logger::THROW("OpenSSL: EVP_DecryptFinal_ex failed");

    
    ++m_decryptionCounter;  // increment counter for next decryption

    return plaintext;
}

} // namespace crypto
} // namespace jm
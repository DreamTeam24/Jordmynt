#include "encrypter.h"

#include "logger/logger.h"
#include <cassert>
#include <limits>
#include <utility>

namespace jm {
namespace crypto {

Bytes::size_type Encrypter::getKeySize()
{
    return s_keySize;
}

Bytes::size_type Encrypter::getInitializationVectorSize()
{
    return s_initializationVectorSize;
}
    
Encrypter::Encrypter(Bytes key,
                     Bytes initializationVector) :
    m_cipherContext{ nullptr },
    m_key{ key },
    m_initializationVector{ initializationVector },
    m_counter{ 0 }
{
    assert(key.size() == s_keySize);
    assert(initializationVector.size() == s_initializationVectorSize);

    logger::DEBUG("Creating a Cipher ...");

    m_cipherContext = EVP_CIPHER_CTX_new();
    if (m_cipherContext == nullptr)
        logger::THROW("OpenSSL: EVP_CIPHER_CTX_new failed");
}

Encrypter::~Encrypter()
{
    EVP_CIPHER_CTX_free(m_cipherContext);
}

Encrypter::Encrypter(Encrypter&& encrypter) noexcept :
    m_cipherContext{ std::exchange(encrypter.m_cipherContext, nullptr) },
    m_key{ std::move(encrypter.m_key) },
    m_initializationVector{ std::move(encrypter.m_initializationVector) },
    m_counter{ encrypter.m_counter }
{
}

Encrypter& Encrypter::operator=(Encrypter&& encrypter) noexcept
{
    m_cipherContext = std::exchange(encrypter.m_cipherContext, nullptr);
    m_key = std::move(encrypter.m_key);
    m_initializationVector = std::move(encrypter.m_initializationVector);
    m_counter = encrypter.m_counter;
    return *this;
}

Bytes Encrypter::encrypt(Bytes const& plaintext,
                         Bytes const& additionalAuthenticatedData,
                         Bytes&       authenticationTag) const
{
    assert(plaintext.size() <= std::numeric_limits<int>::max());

    logger::DEBUG("Encrypting plaintext ...");

    // XOR last 8 bytes of initialization vector with encryption counter
    Bytes sequenceInitializationVector{ m_initializationVector };
    for (Bytes::size_type i = 0; i < 8; i++)
        sequenceInitializationVector[s_initializationVectorSize - 1 - i] ^= ((m_counter >> (i * 8)) & 0xFF);

    if (EVP_EncryptInit_ex(m_cipherContext, s_cipher, nullptr, &(m_key[0]), &(sequenceInitializationVector[0])) != 1)
        logger::THROW("OpenSSL: EVP_EncryptInit_ex failed");
    
    int nBytes(0);      // OpenSSL needs this
    if (EVP_EncryptUpdate(m_cipherContext, nullptr, &nBytes, &additionalAuthenticatedData[0], static_cast<int>(additionalAuthenticatedData.size())) != 1)
        logger::THROW("OpenSSL: EVP_EncryptUpdate failed");
    
    Bytes ciphertext(plaintext.size());
    if (EVP_EncryptUpdate(m_cipherContext, &ciphertext[0], &nBytes, &plaintext[0], static_cast<int>(plaintext.size())) != 1)
        logger::THROW("OpenSSL: EVP_EncryptUpdate failed");
    
    Bytes dummy{ 0 };   // OpenSSL needs this
    if (EVP_EncryptFinal_ex(m_cipherContext, &dummy[0], &nBytes) != 1)  //does nothing in GCM mode
        logger::THROW("OpenSSL: EVP_EncryptFinal_ex failed");
    
    authenticationTag.resize(s_authenticationTagSize);
    if (EVP_CIPHER_CTX_ctrl(m_cipherContext, EVP_CTRL_GCM_GET_TAG, static_cast<int>(s_authenticationTagSize), &authenticationTag[0]) != 1)
        logger::THROW("OpenSSL: EVP_CIPHER_CTX_ctrl failed");

    ++m_counter;        // increment counter for next encryption

    return ciphertext;  // authenticationTag is returned as a parameter
}

} // namespace crypto
} // namespace jm
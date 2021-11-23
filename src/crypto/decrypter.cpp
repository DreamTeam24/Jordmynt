#include "decrypter.h"

#include "logger/logger.h"
#include <cassert>
#include <limits>
#include <utility>

namespace jm {
namespace crypto {

Bytes::size_type Decrypter::getKeySize()
{
    return s_keySize;
}

Bytes::size_type Decrypter::getInitializationVectorSize()
{
    return s_initializationVectorSize;
}

Decrypter::Decrypter(Bytes key,
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

Decrypter::~Decrypter()
{
    EVP_CIPHER_CTX_free(m_cipherContext);
}

Decrypter::Decrypter(Decrypter&& decrypter) noexcept :
    m_cipherContext{ std::exchange(decrypter.m_cipherContext, nullptr) },
    m_key{ std::move(decrypter.m_key) },
    m_initializationVector{ std::move(decrypter.m_initializationVector) },
    m_counter{ decrypter.m_counter }
{
}

Decrypter& Decrypter::operator=(Decrypter&& decrypter) noexcept
{
    m_cipherContext = std::exchange(decrypter.m_cipherContext, nullptr);
    m_key = std::move(decrypter.m_key);
    m_initializationVector = std::move(decrypter.m_initializationVector);
    m_counter = decrypter.m_counter;
    return *this;
}

Bytes Decrypter::decrypt(Bytes const& ciphertext,
                         Bytes const& additionalAuthenticatedData,
                         Bytes        authenticationTag) const
{
    assert(ciphertext.size() <= std::numeric_limits<int>::max());

    logger::DEBUG("Decrypting plaintext ...");

    // XOR last 8 bytes of initialization vector with decryption counter
    Bytes sequenceInitializationVector{ m_initializationVector };
    for (Bytes::size_type i = 0; i < 8; i++)
        sequenceInitializationVector[s_initializationVectorSize - 1 - i] ^= ((m_counter >> (i * 8)) & 0xFF);

    if (EVP_DecryptInit_ex(m_cipherContext, s_cipher, nullptr, &(m_key[0]), &(sequenceInitializationVector[0])) != 1)
        logger::THROW("OpenSSL: EVP_DecryptInit_ex failed");

    int nBytes(0);      // OpenSSL needs this
    if (EVP_DecryptUpdate(m_cipherContext, nullptr, &nBytes, &additionalAuthenticatedData[0], static_cast<int>(additionalAuthenticatedData.size())) != 1)
        logger::THROW("OpenSSL: EVP_DecryptUpdate failed");

    Bytes plaintext(ciphertext.size());
    if (EVP_DecryptUpdate(m_cipherContext, &plaintext[0], &nBytes, &ciphertext[0], static_cast<int>(ciphertext.size())) != 1)
        logger::THROW("OpenSSL: EVP_DecryptUpdate failed");
    
    authenticationTag.resize(s_authenticationTagSize);
    if (EVP_CIPHER_CTX_ctrl(m_cipherContext, EVP_CTRL_GCM_SET_TAG, static_cast<int>(s_authenticationTagSize), &authenticationTag[0]) != 1)
        logger::THROW("OpenSSL: EVP_CIPHER_CTX_ctrl failed");

    Bytes dummy{ 0 };   // OpenSSL needs this
    if (EVP_DecryptFinal_ex(m_cipherContext, &dummy[0], &nBytes) != 1)  //does nothing in GCM mode
        logger::THROW("OpenSSL: EVP_DecryptFinal_ex failed");

    ++m_counter;        // increment counter for next decryption

    return plaintext;
}

} // namespace crypto
} // namespace jm
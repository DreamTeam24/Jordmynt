#include "cipher.h"
#include "opensslerror.h"
#include "logger/logger.h"
#include <openssl/core_names.h>
#include <openssl/evp.h>
#include <array>
#include <utility>
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
std::string const Cipher::s_algorithm{ SN_aes_128_gcm };
// ----------------------------------------------------------------------------
BytesSize const   Cipher::s_keySize{ 16 };
// ----------------------------------------------------------------------------
BytesSize const   Cipher::s_initializationVectorSize{ 12 };
// ----------------------------------------------------------------------------
BytesSize const   Cipher::s_messageAuthenticationCodeSize{ 16 };
// ----------------------------------------------------------------------------
Cipher::Cipher(Bytes key,
               Bytes initializationVector) :
    m_cipher{ EVP_CIPHER_fetch(nullptr,
                               s_algorithm.data(),
                               nullptr) }, 
    m_cipherContext{ EVP_CIPHER_CTX_new() },
    m_key{ std::move(key) },
    m_initializationVector{ std::move(initializationVector) },
    m_encryptionCounter{ 0 },
    m_decryptionCounter{ 0 }
{
    logger::Debug("Creating a Cipher ...");

    if (!m_cipherContext)
        throw OpenSSLError{};
}
// ----------------------------------------------------------------------------
Cipher::~Cipher()
{
}
// ----------------------------------------------------------------------------
Cipher::Cipher(Cipher&& cipher) noexcept :
    m_cipherContext{ std::move(cipher.m_cipherContext) },
    m_key{ std::move(cipher.m_key) },
    m_initializationVector{ std::move(cipher.m_initializationVector) },
    m_encryptionCounter{ cipher.m_encryptionCounter },
    m_decryptionCounter{ cipher.m_decryptionCounter }
{
}
// ----------------------------------------------------------------------------
Cipher& Cipher::operator=(Cipher&& cipher) noexcept
{
    m_cipherContext = std::move(cipher.m_cipherContext);
    m_key = std::move(cipher.m_key);
    m_initializationVector = std::move(cipher.m_initializationVector);
    m_encryptionCounter = cipher.m_encryptionCounter;
    m_decryptionCounter = cipher.m_decryptionCounter;
    return *this;
}
// ----------------------------------------------------------------------------
BytesSize Cipher::getAuthenticatedCiphertextSize(BytesSize plaintextSize)
{
    return plaintextSize + s_messageAuthenticationCodeSize;
}
// ----------------------------------------------------------------------------
BytesSpan Cipher::encryptAndAuthenticate(BytesView  plaintext,
                                         BytesView  associatedData,
                                         BytesSpan& buffer)
{
    logger::Debug("Encrypting and authenticating plaintext ...");

    // XOR last 8 bytes of initialization vector with encryption counter
    Bytes sequenceInitializationVector{ m_initializationVector };
    for (Bytes::size_type i = 0; i < 8; i++)
        sequenceInitializationVector[s_initializationVectorSize - 1 - i] ^= ((m_encryptionCounter >> (i * 8)) & 0xFF);

    if (EVP_EncryptInit_ex2(m_cipherContext.get(),
                            m_cipher.get(),
                            m_key.data(),
                            sequenceInitializationVector.data(),
                            nullptr) != 1)
        throw OpenSSLError{};

    int dummySize{ 0 };
    if (EVP_EncryptUpdate(m_cipherContext.get(),
                          nullptr,
                          &dummySize,
                          associatedData.data(),
                          static_cast<int>(associatedData.size())) != 1)   // ugly...
        throw OpenSSLError{};

    BytesSpan authenticatedCiphertext{ defineVariable(buffer, plaintext.size() + s_messageAuthenticationCodeSize) };
    BytesSpan ciphertext{ authenticatedCiphertext.subspan(0, plaintext.size()) };
    int ciphertextSize{ static_cast<int>(plaintext.size()) };  // ugly...
    if (EVP_EncryptUpdate(m_cipherContext.get(),
                          ciphertext.data(),
                          &ciphertextSize,
                          plaintext.data(),
                          static_cast<int>(plaintext.size())) != 1) // ugly...
        throw OpenSSLError{};

    Bytes dummyBytes{ 0 };
    if (EVP_EncryptFinal_ex(m_cipherContext.get(),
                            dummyBytes.data(),
                            &dummySize) != 1)   // does nothing in gcm mode
        throw OpenSSLError{};

    BytesSpan messageAuthenticationCode{ authenticatedCiphertext.subspan(ciphertext.size(), s_messageAuthenticationCodeSize) };
    std::array<OSSL_PARAM, 2> parameters
    {
        OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG,
                                          messageAuthenticationCode.data(),
                                          s_messageAuthenticationCodeSize),
        OSSL_PARAM_construct_end()
    };

    if (EVP_CIPHER_CTX_get_params(m_cipherContext.get(),
                                  parameters.data()) != 1)
        throw OpenSSLError{};

    ++m_encryptionCounter;

    return authenticatedCiphertext;
}
// ----------------------------------------------------------------------------
BytesSize Cipher::getPlaintextSize(BytesSize authenticatedCiphertextSize)
{
    return authenticatedCiphertextSize - s_messageAuthenticationCodeSize;
}
// ----------------------------------------------------------------------------
BytesSpan Cipher::decrypt(BytesView  authenticatedCiphertext,
                          BytesView  associatedData,
                          BytesSpan& buffer)
{
    logger::Debug("Decrypting ciphertext ...");

    // XOR last 8 bytes of initialization vector with decryption counter
    Bytes sequenceInitializationVector{ m_initializationVector };
    for (Bytes::size_type i = 0; i < 8; i++)
        sequenceInitializationVector[s_initializationVectorSize - 1 - i] ^= ((m_decryptionCounter >> (i * 8)) & 0xFF);

    if (EVP_DecryptInit_ex2(m_cipherContext.get(),
                            m_cipher.get(),
                            m_key.data(),
                            sequenceInitializationVector.data(),
                            nullptr) != 1)
        throw OpenSSLError{};

    int dummySize{ 0 };
    if (EVP_DecryptUpdate(m_cipherContext.get(),
                          nullptr,
                          &dummySize,
                          associatedData.data(),
                          static_cast<int>(associatedData.size())) != 1)   // ugly...
        throw OpenSSLError{};

    BytesView ciphertext{ authenticatedCiphertext.subspan(
        0, authenticatedCiphertext.size() - s_messageAuthenticationCodeSize) };
    BytesSpan plaintext{ defineVariable(buffer, ciphertext.size()) };
    int plaintextSize{ static_cast<int>(buffer.size()) };   // ugly...
    if (EVP_DecryptUpdate(m_cipherContext.get(),
                          plaintext.data(),
                          &plaintextSize,
                          ciphertext.data(),
                          static_cast<int>(ciphertext.size())) != 1)    // ugly...
        throw OpenSSLError{};

    BytesView messageAuthenticationCode{ authenticatedCiphertext.subspan(
        ciphertext.size(), s_messageAuthenticationCodeSize) };
    std::array<OSSL_PARAM const, 2> parameters
    {
        OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG,
                                          const_cast<Byte*>(messageAuthenticationCode.data()),  // ugly...
                                          s_messageAuthenticationCodeSize),
        OSSL_PARAM_construct_end()
    };

    if (EVP_CIPHER_CTX_set_params(m_cipherContext.get(),
                                  parameters.data()) != 1)
        throw OpenSSLError{};

    Bytes dummyBytes{ 0 };
    if (EVP_DecryptFinal_ex(m_cipherContext.get(),
                            dummyBytes.data(),
                            &dummySize) != 1)   //does nothing in GCM mode
        throw OpenSSLError{};

    ++m_decryptionCounter;

    return plaintext;
}
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
#ifndef JM_CRYPTO_CIPHER_H
#define JM_CRYPTO_CIPHER_H
// ----------------------------------------------------------------------------
#include "pointer.h"
#include "common/byte.h"
#include <string>
// ----------------------------------------------------------------------------
typedef struct evp_cipher_st     EVP_CIPHER;
typedef struct evp_cipher_ctx_st EVP_CIPHER_CTX;
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
// An algorithm to provide confidentiality, authenticity, and data integrity.
class Cipher
{
public:
    Cipher(Bytes key,
           Bytes initializationVector);
    ~Cipher();

    Cipher(Cipher const& cipher) = delete;
    Cipher& operator=(Cipher const& cipher) = delete;

    Cipher(Cipher&& encrypter) noexcept;
    Cipher& operator=(Cipher&& encrypter) noexcept;

    static BytesSize getAuthenticatedCiphertextSize(BytesSize plaintextSize);
    BytesSpan encryptAndAuthenticate(BytesView  plaintext,
                                     BytesView  associatedData,
                                     BytesSpan& buffer);
    
    static BytesSize getPlaintextSize(BytesSize authenticatedCiphertextSize);
    BytesSpan decrypt(BytesView  authenticatedCiphertext,
                      BytesView  associatedData,
                      BytesSpan& buffer);

private:
    static std::string const s_algorithm;
    static BytesSize const   s_keySize;
    static BytesSize const   s_initializationVectorSize;
    static BytesSize const   s_messageAuthenticationCodeSize;

    Pointer<EVP_CIPHER>     m_cipher; 
    Pointer<EVP_CIPHER_CTX> m_cipherContext;
    Bytes                   m_key;
    Bytes                   m_initializationVector;
    unsigned long long      m_encryptionCounter;
    unsigned long long      m_decryptionCounter;
};
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_CRYPTO_CIPHER_H
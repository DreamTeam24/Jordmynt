#ifndef JM_CRYPTO_CIPHER_H
#define JM_CRYPTO_CIPHER_H

#include "bytes.h"
#include <openssl/evp.h>
#include <string>

namespace jm {
namespace crypto {

// C++ wrapper class around OpenSSL functions involving EVP_CIPHER_CTX
// to encrypt and decrypt messages with a pseudo random key and initialization vector.
class Cipher
{
public:
    Cipher(Bytes key,
           Bytes initializationVector);
    ~Cipher();

    Cipher(Cipher const& copyFromMe) = delete;                  // non-copyable
    Cipher& operator=(Cipher const& assignFromMe) = delete;     // non-copyable

    Bytes encrypt(Bytes const& plaintext,
                  Bytes const& additionalAuthenticatedData,
                  Bytes&       authenticationTag = Bytes{});    // extra return value
    Bytes decrypt(Bytes const& ciphertext,
                  Bytes const& additionalAuthenticatedData,
                  Bytes&       authenticationTag);              // parameter, non-const because of OpenSSL

private:
    static inline EVP_CIPHER const* const s_cipher{ EVP_aes_128_gcm() };
    static constexpr Bytes::size_type s_keySize{ 16 };
    static constexpr Bytes::size_type s_initializationVectorSize{ 12 };
    static constexpr Bytes::size_type s_authenticationTagSize{ 16 };

    EVP_CIPHER_CTX*        m_cipherContext;
    Bytes                  m_key;
    Bytes                  m_initializationVector;
    unsigned long long int m_encryptionCounter;
    unsigned long long int m_decryptionCounter;
};

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_CIPHER_H
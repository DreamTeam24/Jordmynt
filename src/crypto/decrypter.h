#ifndef JM_CRYPTO_DECRYPTER_H
#define JM_CRYPTO_DECRYPTER_H

#include <openssl/evp.h>
#include <vector>

namespace jm {
namespace crypto {

using Bytes = std::vector<unsigned char>;

// C++ wrapper class around OpenSSL functions involving EVP_CIPHER_CTX
// to decrypt messages with a pseudo random key and initialization vector.

class Decrypter
{
public:
    static Bytes::size_type getKeySize();
    static Bytes::size_type getInitializationVectorSize();
    
    Decrypter(Bytes key,
              Bytes initializationVector);
    ~Decrypter();

    Decrypter(Decrypter const& decrypter) = delete;             // not copyable
    Decrypter& operator=(Decrypter const& decrypter) = delete;  // not copy-assignable

    Decrypter(Decrypter&& decrypter) noexcept;                  // movable
    Decrypter& operator=(Decrypter&& decrypter) noexcept;       // move-assignable

    Bytes decrypt(Bytes const& ciphertext,
                  Bytes const& additionalAuthenticatedData,
                  Bytes        authenticationTag) const;        // not const& because of OpenSSL

private:
    static inline EVP_CIPHER const* const s_cipher{ EVP_aes_128_gcm() };
    static constexpr Bytes::size_type     s_keySize{ 16 };
    static constexpr Bytes::size_type     s_initializationVectorSize{ 12 };
    static constexpr Bytes::size_type     s_authenticationTagSize{ 16 };

    EVP_CIPHER_CTX*            m_cipherContext;
    Bytes                      m_key;
    Bytes                      m_initializationVector;
    unsigned long long mutable m_counter;
};

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_DECRYPTER_H
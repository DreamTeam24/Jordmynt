#ifndef JM_CRYPTO_ENCRYPTER_H
#define JM_CRYPTO_ENCRYPTER_H

#include <openssl/evp.h>
#include <vector>

namespace jm {
namespace crypto {

using Bytes = std::vector<unsigned char>;

class Encrypter
{
public:
    static Bytes::size_type getKeySize();
    static Bytes::size_type getInitializationVectorSize();
    
    Encrypter(Bytes key,
              Bytes initializationVector);
    ~Encrypter();

    Encrypter(Encrypter const& encrypter) = delete;             // not copyable
    Encrypter& operator=(Encrypter const& encrypter) = delete;  // not copy-assignable

    Encrypter(Encrypter&& encrypter) noexcept;                  // movable
    Encrypter& operator=(Encrypter&& encrypter) noexcept;       // move-assignable

    Bytes encrypt(Bytes const& plaintext,
                  Bytes const& additionalAuthenticatedData,
                  Bytes&       authenticationTag = Bytes{}) const;  // extra return value

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

#endif // JM_CRYPTO_ENCRYPTER_H
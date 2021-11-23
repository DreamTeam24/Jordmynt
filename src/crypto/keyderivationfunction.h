#ifndef JM_CRYPTO_KEYDERIVATIONFUNCTION_H
#define JM_CRYPTO_KEYDERIVATIONFUNCTION_H

#include <openssl/evp.h>
#include <vector>

namespace jm {
namespace crypto {

using Bytes = std::vector<unsigned char>;

// C++ wrapper class around OpenSSL functions involving EVP_PKEY_CTX
// to derive pseudo random keys from a shared secret.

class KeyDerivationFunction
{
public:
    static Bytes::size_type getPseudoRandomKeySize(); 
    
    KeyDerivationFunction();
    ~KeyDerivationFunction();

    KeyDerivationFunction(KeyDerivationFunction const& keyDerivationFunction) = delete;             // not copyable
    KeyDerivationFunction& operator=(KeyDerivationFunction const& keyDerivationFunction) = delete;  // not copy-assignable

    KeyDerivationFunction(KeyDerivationFunction&& keyDerivationFunction) noexcept;                  // movable
    KeyDerivationFunction& operator=(KeyDerivationFunction&& keyDerivationFunction) noexcept;       // move-assignable

    Bytes extractPseudoRandomKey(Bytes inputKeyingMaterial,         // no const& because of OpenSSL
                                 Bytes salt) const;                 // no const& because of OpenSSL
    Bytes expandPseudoRandomKey(Bytes            pseudoRandomKey,   // no const& because of OpenSSL
                                Bytes const&     label,
                                Bytes const&     info,
                                Bytes::size_type expandedKeySize) const;

private:
    static inline EVP_MD const* const s_hashFunction{ EVP_sha256() }; 
    static constexpr int              s_id{ EVP_PKEY_HKDF };
    static constexpr Bytes::size_type s_pseudoRandomKeySize{ 32 };

    EVP_PKEY_CTX* m_keyContext;
};

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_KEYDERIVATIONFUNCTION_H
#ifndef JM_CRYPTO_KEYDERIVATIONFUNCTION_H
#define JM_CRYPTO_KEYDERIVATIONFUNCTION_H

#include "bytes.h"
#include <openssl/evp.h>
#include <string>

namespace jm {
namespace crypto {

// C++ wrapper class around OpenSSL functions involving EVP_PKEY_CTX
// to derive pseudo random keys from a shared secret.
class KeyDerivationFunction
{
public:
    KeyDerivationFunction();
    ~KeyDerivationFunction();

    KeyDerivationFunction(KeyDerivationFunction const& copyFromMe) = delete;              // non-copyable
    KeyDerivationFunction& operator=(KeyDerivationFunction const& assignFromMe) = delete; // non-copyable

    Bytes extractPseudoRandomKey(Bytes& inputKeyingMaterial,
                                 Bytes& salt) const;
    Bytes expandPseudoRandomKey(Bytes& pseudoRandomKey,
                                Bytes& labelledInfo,
                                Bytes::size_type expandedKeySize) const;

private:
    static constexpr int              s_id{ EVP_PKEY_HKDF };
    static inline EVP_MD const* const s_hashFunction{ EVP_sha256() };
    static constexpr Bytes::size_type s_pseudoRandomKeySize{ 32 };

    EVP_PKEY_CTX* m_keyContext;
};

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_KEYDERIVATIONFUNCTION_H
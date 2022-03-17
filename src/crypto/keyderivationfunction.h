#ifndef JM_CRYPTO_KEYDERIVATIONFUNCTION_H
#define JM_CRYPTO_KEYDERIVATIONFUNCTION_H
// ----------------------------------------------------------------------------
#include "pointer.h"
#include "common/byte.h"
#include <string>
// ----------------------------------------------------------------------------
typedef struct evp_kdf_st     EVP_KDF;
typedef struct evp_kdf_ctx_st EVP_KDF_CTX;
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
// A function to derive pseudo-random keys from another key.
class KeyDerivationFunction
{
public:
    KeyDerivationFunction();
    ~KeyDerivationFunction();

    KeyDerivationFunction(KeyDerivationFunction const& keyDerivationFunction) = delete;
    KeyDerivationFunction& operator=(KeyDerivationFunction const& keyDerivationFunction) = delete;

    KeyDerivationFunction(KeyDerivationFunction&& keyDerivationFunction) noexcept;
    KeyDerivationFunction& operator=(KeyDerivationFunction&& keyDerivationFunction) noexcept;

    static BytesSize getExtractedKeySize();
    BytesSpan extractKey(BytesView  key,
                         BytesView  salt,
                         BytesSpan& buffer) const;
    
    BytesSpan expandKey(BytesView  key,
                        BytesView  info,
                        BytesSize  expandedKeySize,
                        BytesSpan& buffer) const;

private:
    static std::string const s_hashAlgorithm;
    static std::string const s_macAlgorithm;
    static int const         s_extractMode;
    static int const         s_expandMode;
    static BytesSize const   s_extractedKeySize;

    Pointer<EVP_KDF>     m_function;
    Pointer<EVP_KDF_CTX> m_functionContext;     // depends on m_function
    
};
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_CRYPTO_KEYDERIVATIONFUNCTION_H
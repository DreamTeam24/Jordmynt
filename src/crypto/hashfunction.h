#ifndef JM_CRYPTO_HASHFUNCTION_H
#define JM_CRYPTO_HASHFUNCTION_H
// ----------------------------------------------------------------------------
#include "pointer.h"
#include "common/byte.h"
#include <string>
// ----------------------------------------------------------------------------
typedef struct evp_md_st     EVP_MD;
typedef struct evp_md_ctx_st EVP_MD_CTX;
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
// A function to map arbitrary-size messages to a fixed-size message.
class HashFunction
{
public:
    HashFunction();
    ~HashFunction();

    HashFunction(HashFunction const& hashFunction) = delete;
    HashFunction& operator=(HashFunction const& hashFunction) = delete;

    HashFunction(HashFunction&& hashFunction) noexcept;
    HashFunction& operator=(HashFunction&& hashFunction) noexcept;

    void digest(BytesView message) const;

    static BytesSize getDigestSize();
    BytesSpan getDigest(BytesSpan& buffer) const;

private:
    static std::string const s_algorithm;
    static BytesSize const   s_digestSize;

    Pointer<EVP_MD>     m_function;
    Pointer<EVP_MD_CTX> m_functionContext;
};
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_CRYPTO_HASHFUNCTION_H
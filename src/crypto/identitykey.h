#ifndef JM_CRYPTO_IDENTITYKEY_H
#define JM_CRYPTO_IDENTITYKEY_H
// ----------------------------------------------------------------------------
#include "pointer.h"
#include "common/byte.h"
#include <string>
#include <string_view>
// ----------------------------------------------------------------------------
typedef struct evp_pkey_st     EVP_PKEY;
typedef struct evp_pkey_ctx_st EVP_PKEY_CTX;
typedef struct evp_md_ctx_st   EVP_MD_CTX;
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
// A long-term use keypair to provide authenticity.
class IdentityKey
{
public:
    IdentityKey();
    IdentityKey(BytesView privateKey);
    IdentityKey(std::string_view privateKeyFilename);
    ~IdentityKey();

    IdentityKey(IdentityKey const& identityKey) = delete;
    IdentityKey& operator=(IdentityKey const& identityKey) = delete;

    IdentityKey(IdentityKey&& identityKey) noexcept;
    IdentityKey& operator=(IdentityKey&& identityKey) noexcept;

    static BytesSize getPublicKeySize();
    BytesSpan getPublicKey(BytesSpan& buffer) const;
    
    static BytesSize getSignatureSize();
    BytesSpan sign(BytesView  message,
                   BytesSpan& buffer) const;

    static bool verify(BytesView message,
                       BytesView signature,
                       BytesView peerPublicKey);

private:
    static std::string const s_algorithm;
    static BytesSize const   s_publicKeySize;
    static BytesSize const   s_signatureSize;

    Pointer<EVP_PKEY>     m_key;
    Pointer<EVP_PKEY_CTX> m_keyContext;     // depends on m_key
    Pointer<EVP_MD_CTX>   m_signContext;    // depends on m_key
};
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_CRYPTO_IDENTITYKEY_H
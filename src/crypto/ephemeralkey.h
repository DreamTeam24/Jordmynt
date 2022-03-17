#ifndef JM_CRYPTO_EPHEMERALKEY_H
#define JM_CRYPTO_EPHEMERALKEY_H
// ----------------------------------------------------------------------------
#include "pointer.h"
#include "common/byte.h"
#include <string>
#include <string_view>
// ----------------------------------------------------------------------------
typedef struct evp_pkey_st     EVP_PKEY;
typedef struct evp_pkey_ctx_st EVP_PKEY_CTX;
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
// A single-use keypair to provide confidentiality.
class EphemeralKey
{
public:
    EphemeralKey();
    EphemeralKey(BytesView privateKey);
    EphemeralKey(std::string_view privateKeyFilename);
    ~EphemeralKey();

    EphemeralKey(EphemeralKey const& ephemeralKey) = delete;
    EphemeralKey& operator=(EphemeralKey const& ephemeralKey) = delete;

    EphemeralKey(EphemeralKey&& ephemeralKey) noexcept;
    EphemeralKey& operator=(EphemeralKey&& ephemeralKey) noexcept;

    static BytesSize getPublicKeySize();
    BytesSpan getPublicKey(BytesSpan& buffer) const;

    static BytesSize getSharedSecretSize();
    BytesSpan deriveSharedSecret(BytesView  peerPublicKey,
                                 BytesSpan& buffer) const;

private:
    static std::string const s_algorithm;
    static BytesSize const   s_publicKeySize;
    static BytesSize const   s_sharedSecretSize;

    Pointer<EVP_PKEY>     m_key;
    Pointer<EVP_PKEY_CTX> m_keyContext;         // depends on m_key
};
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_CRYPTO_EPHEMERALKEY_H
#include "pointer.h"
#include <openssl/evp.h>
#include <openssl/kdf.h>
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
template <>
void Deleter<EVP_PKEY>::operator()(EVP_PKEY* evp_pkey) const
{
    EVP_PKEY_free(evp_pkey);
};
// ----------------------------------------------------------------------------
template <>
void Deleter<EVP_PKEY_CTX>::operator()(EVP_PKEY_CTX* evp_pkey_ctx) const
{
    EVP_PKEY_CTX_free(evp_pkey_ctx);
};
// ----------------------------------------------------------------------------
template <>
void Deleter<BIO>::operator()(BIO* bio) const
{
    BIO_free(bio);
};
// ----------------------------------------------------------------------------
template <>
void Deleter<EVP_MD>::operator()(EVP_MD* evp_md) const
{
    EVP_MD_free(evp_md);
};
// ----------------------------------------------------------------------------
template <>
void Deleter<EVP_MD_CTX>::operator()(EVP_MD_CTX* evp_md_ctx) const
{
    EVP_MD_CTX_free(evp_md_ctx);
};
// ----------------------------------------------------------------------------
template <>
void Deleter<EVP_CIPHER>::operator()(EVP_CIPHER* evp_cipher) const
{
    EVP_CIPHER_free(evp_cipher);
};
// ----------------------------------------------------------------------------
template <>
void Deleter<EVP_CIPHER_CTX>::operator()(EVP_CIPHER_CTX* evp_cipher_ctx) const
{
    EVP_CIPHER_CTX_free(evp_cipher_ctx);
};
// ----------------------------------------------------------------------------
template <>
void Deleter<EVP_KDF>::operator()(EVP_KDF* evp_kdf) const
{
    EVP_KDF_free(evp_kdf);
};
// ----------------------------------------------------------------------------
template <>
void Deleter<EVP_KDF_CTX>::operator()(EVP_KDF_CTX* evp_kdf_ctx) const
{
    EVP_KDF_CTX_free(evp_kdf_ctx);
};
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
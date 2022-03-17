#include "keyderivationfunction.h"
#include "opensslerror.h"
#include "logger/logger.h"
#include <openssl/core_names.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <algorithm>
#include <array>
#include <utility>
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
std::string const KeyDerivationFunction::s_hashAlgorithm{ SN_sha256 };
// ----------------------------------------------------------------------------
std::string const KeyDerivationFunction::s_macAlgorithm{ SN_hkdf };
// ----------------------------------------------------------------------------
int const         KeyDerivationFunction::s_extractMode{ EVP_KDF_HKDF_MODE_EXTRACT_ONLY };
// ----------------------------------------------------------------------------
int const         KeyDerivationFunction::s_expandMode{ EVP_KDF_HKDF_MODE_EXPAND_ONLY };
// ----------------------------------------------------------------------------
BytesSize const   KeyDerivationFunction::s_extractedKeySize{ 32 };
// ----------------------------------------------------------------------------
KeyDerivationFunction::KeyDerivationFunction() :
    m_function{ EVP_KDF_fetch(nullptr,
                              s_macAlgorithm.data(),
                              nullptr) },
    m_functionContext{ EVP_KDF_CTX_new(m_function.get()) }
{
    logger::Debug("Creating a KeyDerivationFunction ...");

    if (!m_function)
        throw OpenSSLError{};

    if (!m_functionContext)
        throw OpenSSLError{};
}
// ----------------------------------------------------------------------------
KeyDerivationFunction::~KeyDerivationFunction()
{
}
// ----------------------------------------------------------------------------
KeyDerivationFunction::KeyDerivationFunction(KeyDerivationFunction&& keyDerivationFunction) noexcept :
    m_function{ std::move(keyDerivationFunction.m_function) },
    m_functionContext{ std::move(keyDerivationFunction.m_functionContext) }
{
}
// ----------------------------------------------------------------------------
KeyDerivationFunction& KeyDerivationFunction::operator=(KeyDerivationFunction&& keyDerivationFunction) noexcept
{
    m_function = std::move(keyDerivationFunction.m_function);
    m_functionContext = std::move(keyDerivationFunction.m_functionContext);
    return *this;
}
// ----------------------------------------------------------------------------
BytesSize KeyDerivationFunction::getExtractedKeySize()
{
    return s_extractedKeySize;
}
// ----------------------------------------------------------------------------
BytesSpan KeyDerivationFunction::extractKey(BytesView  key,
                                            BytesView  salt,
                                            BytesSpan& buffer) const
{
    logger::Debug("Extracting a pseudo-random key ...");

    // https://github.com/openssl/openssl/blob/master/demos/kdf/pbkdf2.c
    std::array<OSSL_PARAM, 5> parameters
    {
        OSSL_PARAM_construct_int(OSSL_KDF_PARAM_MODE,
                                 const_cast<int*>(&s_extractMode)),   // ugly...
        OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST,
                                         const_cast<char*>(s_hashAlgorithm.data()),   // ugly...
                                         s_hashAlgorithm.size()),
        OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_KEY,
                                          const_cast<Byte*>(key.data()),   // ugly...
                                          key.size()),
        OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SALT,
                                          const_cast<Byte*>(salt.data()),   // ugly...
                                          salt.size()),
        OSSL_PARAM_construct_end()
    };

    BytesSpan extractedKey{ defineVariable(buffer, s_extractedKeySize) };
    if (EVP_KDF_derive(m_functionContext.get(),
                       extractedKey.data(),
                       s_extractedKeySize,
                       parameters.data()) != 1)
        throw OpenSSLError{};

    return extractedKey;
}
// ----------------------------------------------------------------------------
BytesSpan KeyDerivationFunction::expandKey(BytesView  key,
                                           BytesView  info,
                                           BytesSize  expandedKeySize,
                                           BytesSpan& buffer) const
{
    logger::Debug("Expanding a pseudo-random key ...");
    
    // https://github.com/openssl/openssl/blob/master/demos/kdf/pbkdf2.c
    std::array<OSSL_PARAM, 5> parameters
    {
        OSSL_PARAM_construct_int(OSSL_KDF_PARAM_MODE,
                                 const_cast<int*>(&s_expandMode)),   // ugly...
        OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST,
                                         const_cast<char*>(s_hashAlgorithm.data()),    // ugly...
                                         s_hashAlgorithm.size()),
        OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_KEY,
                                          const_cast<Byte*>(key.data()),       // ugly...
                                          key.size()),
        OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_INFO,
                                          const_cast<Byte*>(info.data()),   // ugly...
                                          info.size()),
        OSSL_PARAM_construct_end()
    };
    
    BytesSpan expandedKey{ defineVariable(buffer, expandedKeySize) };
    if (EVP_KDF_derive(m_functionContext.get(),
                       expandedKey.data(),
                       expandedKeySize,
                       parameters.data()) != 1)
        throw OpenSSLError{};

    return expandedKey;
}
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
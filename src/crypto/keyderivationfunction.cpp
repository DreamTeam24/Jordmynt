#include "keyderivationfunction.h"
#include "logger/logger.h"
#include <openssl/kdf.h>
#include <cassert>
#include <limits>

namespace jm {
namespace crypto {

KeyDerivationFunction::KeyDerivationFunction() :
    m_keyContext{ nullptr }
{
    logger::DEBUG("Creating a KeyDerivationFunction ...");

    m_keyContext = EVP_PKEY_CTX_new_id(s_id, nullptr);
    if (m_keyContext == nullptr)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_new_id failed");
}

KeyDerivationFunction::~KeyDerivationFunction()
{
    EVP_PKEY_CTX_free(m_keyContext);
}

Bytes KeyDerivationFunction::extractPseudoRandomKey(Bytes& inputKeyingMaterial,
                                                    Bytes& salt) const
{
    assert(inputKeyingMaterial.size() <= std::numeric_limits<int>::max());
    assert(salt.size() <= std::numeric_limits<int>::max());

    logger::DEBUG("Deriving a pseudo-random key ..."); 

    if (EVP_PKEY_derive_init(m_keyContext) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_derive_init failed");
    
    if (EVP_PKEY_CTX_set_hkdf_md(m_keyContext, s_hashFunction) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_set_hkdf_md failed");
    
    if (EVP_PKEY_CTX_hkdf_mode(m_keyContext, EVP_PKEY_HKDEF_MODE_EXTRACT_ONLY) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_hkdf_mode failed");
    
    if (EVP_PKEY_CTX_set1_hkdf_key(m_keyContext, &inputKeyingMaterial[0], static_cast<int>(inputKeyingMaterial.size())) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_set1_hkdf_key failed");
    
    if (EVP_PKEY_CTX_set1_hkdf_salt(m_keyContext, (salt.empty() ? nullptr : &salt[0]), static_cast<int>(salt.size())) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_set1_hkdf_salt failed");
    
    Bytes::size_type pseudoRandomKeySize{ s_pseudoRandomKeySize };  // OpenSSL needs this
    Bytes pseudoRandomKey(pseudoRandomKeySize);                     // {}-initialization does not set the size
    if (EVP_PKEY_derive(m_keyContext, &pseudoRandomKey[0], &pseudoRandomKeySize) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_derive failed");

    return pseudoRandomKey;
}

Bytes KeyDerivationFunction::expandPseudoRandomKey(Bytes& pseudoRandomKey,
                                                   Bytes& labelledInfo,
                                                   Bytes::size_type expandedKeySize) const
{
    assert(pseudoRandomKey.size() == s_pseudoRandomKeySize); 
    assert(labelledInfo.size() <= std::numeric_limits<int>::max());

    logger::DEBUG("Expanding a pseudo-random key ...");

    if (EVP_PKEY_derive_init(m_keyContext) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_derive_init failed");
    
    if (EVP_PKEY_CTX_set_hkdf_md(m_keyContext, s_hashFunction) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_set_hkdf_md failed");
    
    if (EVP_PKEY_CTX_hkdf_mode(m_keyContext, EVP_PKEY_HKDEF_MODE_EXPAND_ONLY) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_hkdf_mode failed");
    
    if (EVP_PKEY_CTX_set1_hkdf_key(m_keyContext, &pseudoRandomKey[0], static_cast<int>(pseudoRandomKey.size())) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_set1_hkdf_key failed");
    
    if (EVP_PKEY_CTX_add1_hkdf_info(m_keyContext, &labelledInfo[0], static_cast<int>(labelledInfo.size())) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_CTX_add1_hkdf_info failed");
    
    Bytes expandedKey(expandedKeySize);     // {}-initialization does not set the size
    if (EVP_PKEY_derive(m_keyContext, &expandedKey[0], &expandedKeySize) != 1)
        logger::THROW("OpenSSL: EVP_PKEY_derive failed");

    return expandedKey;
}

} // namespace crypto
} // namespace jm
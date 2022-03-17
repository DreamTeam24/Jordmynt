#include "hashfunction.h"
#include "opensslerror.h"
#include "logger/logger.h"
#include <openssl/evp.h>
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
std::string const HashFunction::s_algorithm{ SN_sha256 };
// ----------------------------------------------------------------------------
BytesSize const   HashFunction::s_digestSize{ 32 };
// ----------------------------------------------------------------------------
HashFunction::HashFunction() :
    m_function{ EVP_MD_fetch(nullptr,
                             s_algorithm.data(),
                             nullptr) },
    m_functionContext{ EVP_MD_CTX_new() }
{
    logger::Debug("Creating a HashFunction ...");

    if (!m_function)
        throw OpenSSLError{};

    if (!m_functionContext)
        throw OpenSSLError{};
    
    if (EVP_DigestInit_ex2(m_functionContext.get(),
                           m_function.get(),
                           nullptr) != 1)
        throw OpenSSLError{};
}
// ----------------------------------------------------------------------------
HashFunction::~HashFunction()
{
}
// ----------------------------------------------------------------------------
HashFunction::HashFunction(HashFunction&& hashFunction) noexcept :
    m_function{ std::move(hashFunction.m_function) },
    m_functionContext{ std::move(hashFunction.m_functionContext) }
{
}
// ----------------------------------------------------------------------------
HashFunction& HashFunction::operator=(HashFunction&& hashFunction) noexcept
{
    m_function = std::move(hashFunction.m_function);
    m_functionContext = std::move(hashFunction.m_functionContext);
    return *this;
}
// ----------------------------------------------------------------------------
void HashFunction::digest(BytesView message) const
{
    logger::Debug("Digesting a message ...");
    
    if (EVP_DigestUpdate(m_functionContext.get(),
                         message.data(),
                         message.size()) != 1)
        throw OpenSSLError{};
}
// ----------------------------------------------------------------------------
BytesSize HashFunction::getDigestSize()
{
    return s_digestSize;
}
// ----------------------------------------------------------------------------
BytesSpan HashFunction::getDigest(BytesSpan& buffer) const
{
    logger::Debug("Getting a digest ...");

    // Get result of previous calls to digest(message)
    BytesSpan digest{ defineVariable(buffer, s_digestSize) };
    unsigned int digestSize{ static_cast<BytesSize>(s_digestSize) };    // ugly...
    if (EVP_DigestFinal_ex(m_functionContext.get(),
                           digest.data(),
                           &digestSize) != 1)
        throw OpenSSLError{};

    // Make ready for a new digest
    if (EVP_DigestInit_ex2(m_functionContext.get(),
                           m_function.get(),
                           nullptr) != 1)
        throw OpenSSLError{};

    return digest;
}
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
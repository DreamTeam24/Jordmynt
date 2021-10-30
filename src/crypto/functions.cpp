#include "functions.h"
#include "logger/logger.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cassert>
#include <limits>

namespace jm {
namespace crypto {

Bytes generateRandomBytes(Bytes::size_type nBytes)
{
    assert(nBytes <= std::numeric_limits<int>::max());

    logger::DEBUG("Generating random bytes ...");
    
    Bytes randomBytes(nBytes);      // {}-initialization does not set the size

    if (RAND_bytes(&randomBytes[0], static_cast<int>(nBytes)) != 1)
        logger::THROW("OpenSSL: RAND_bytes failed");

    return randomBytes;
}

Bytes hash(Bytes const& data)
{
    logger::DEBUG("Hashing data ...");
    
    unsigned int hashSize{ 32 };    // OpenSSL demands unsigned int;  SHA-256 --> 256 bits = 32 Bytes
    Bytes hash(hashSize);           // {}-initialization does not set the size

    if (EVP_Digest((data.empty() ? nullptr : &data[0]), data.size(), &hash[0], &hashSize, EVP_sha256(), nullptr) != 1)
        logger::THROW("OpenSSL: EVP_Digest failed");

    return hash;
}

} // namespace crypto
} // namespace jm
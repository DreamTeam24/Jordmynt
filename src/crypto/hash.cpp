#include "hash.h"

#include "logger/logger.h"
#include <openssl/evp.h>

namespace jm {
namespace crypto {

Bytes hash(Bytes const& data)
{
    logger::DEBUG("Hashing data ...");
    
    unsigned int hashSize{ 32 };    // OpenSSL demands unsigned int;  SHA-256 --> 256 bits = 32 Bytes
    Bytes hash(hashSize);

    if (EVP_Digest((data.empty() ? nullptr : &data[0]), data.size(), &hash[0], &hashSize, EVP_sha256(), nullptr) != 1)
        logger::THROW("OpenSSL: EVP_Digest failed");

    return hash;
}

} // namespace crypto
} // namespace jm
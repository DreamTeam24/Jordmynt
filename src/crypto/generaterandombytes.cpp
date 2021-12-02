#include "generaterandombytes.h"

#include "logger/logger.h"
#include <openssl/rand.h>
#include <cassert>
#include <limits>

namespace jm {
namespace crypto {

Bytes generateRandomBytes(Bytes::size_type nBytes)
{
    assert(nBytes <= std::numeric_limits<int>::max());

    logger::DEBUG("Generating random bytes ...");
    
    Bytes randomBytes(nBytes);

    if (RAND_bytes(&randomBytes[0], static_cast<int>(nBytes)) != 1)
        logger::THROW("OpenSSL: RAND_bytes failed");

    return randomBytes;
}

} // namespace crypto
} // namespace jm
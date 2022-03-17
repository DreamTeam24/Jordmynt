#include "generaterandombytes.h"
#include "opensslerror.h"
#include "logger/logger.h"
#include <openssl/rand.h>
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
BytesSpan generateRandomBytes(BytesSize  randomBytesSize,
                              BytesSpan& buffer)
{
    logger::Debug("Generating random bytes ...");

    BytesSpan randomBytes{ defineVariable(buffer, randomBytesSize) };
    if (RAND_bytes(randomBytes.data(),
                   static_cast<int>(randomBytesSize)) != 1)
        throw OpenSSLError{};

    return randomBytes;
}
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
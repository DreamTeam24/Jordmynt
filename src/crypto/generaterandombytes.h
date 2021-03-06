#ifndef JM_CRYPTO_GENERATERANDOMBYTES_H
#define JM_CRYPTO_GENERATERANDOMBYTES_H
// ----------------------------------------------------------------------------
#include "common/byte.h"
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
BytesSpan generateRandomBytes(BytesSize  randomBytesSize,
                              BytesSpan& buffer);
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_CRYPTO_GENERATERANDOMBYTES_H
#ifndef JM_CRYPTO_FUNCTIONS_H
#define JM_CRYPTO_FUNCTIONS_H

#include "bytes.h"

namespace jm {
namespace crypto {

Bytes generateRandomBytes(Bytes::size_type nBytes);
Bytes hash(Bytes const& data);

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_FUNCTIONS_H
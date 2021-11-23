#ifndef JM_CRYPTO_FUNCTIONS_H
#define JM_CRYPTO_FUNCTIONS_H

#include <vector>

namespace jm {
namespace crypto {

using Bytes = std::vector<unsigned char>;

Bytes generateRandomBytes(Bytes::size_type nBytes);
Bytes hash(Bytes const& data);

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_FUNCTIONS_H
#ifndef JM_CRYPTO_HASH_H
#define JM_CRYPTO_HASH_H

#include <vector>

namespace jm {
namespace crypto {

using Bytes = std::vector<unsigned char>;

Bytes hash(Bytes const& data);

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_HASH_H
#ifndef JM_CRYPTO_GENERATERANDOMBYTES_H
#define JM_CRYPTO_GENERATERANDOMBYTES_H

#include <vector>

namespace jm {
namespace crypto {

using Bytes = std::vector<unsigned char>;

Bytes generateRandomBytes(Bytes::size_type nBytes);

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_GENERATERANDOMBYTES_H
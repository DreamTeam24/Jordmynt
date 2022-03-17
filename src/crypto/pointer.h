#ifndef JM_CRYPTO_POINTER_H
#define JM_CRYPTO_POINTER_H
// ----------------------------------------------------------------------------
#include <memory>
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
template<typename T>
struct Deleter
{
    void operator()(T* t) const;
};
// ----------------------------------------------------------------------------
template<typename T>
using Pointer = std::unique_ptr<T, Deleter<T>>;
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_CRYPTO_POINTER_H
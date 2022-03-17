#ifndef JM_COMMON_BYTE_H
#define JM_COMMON_BYTE_H
// ----------------------------------------------------------------------------
#include <span>
#include <vector>
// ----------------------------------------------------------------------------
namespace jm {
// ----------------------------------------------------------------------------
using Byte = unsigned char;                 // OpenSSL demands unsigned char
using Bytes = std::vector<Byte>;            // dynamic contiguous memory
using BytesSize = Bytes::size_type;
using BytesView = std::span<Byte const>;    // OpenSSL demands pointer and size
using BytesSpan = std::span<Byte>;          // OpenSSL demands pointer and size
// ----------------------------------------------------------------------------
// Defines a variable at the beginning of the buffer and then
// sets the beginning of the buffer past the end of that variable.
BytesSpan defineVariable(BytesSpan& buffer,
                         BytesSize  variableSize);
// ----------------------------------------------------------------------------
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_COMMON_BYTE_H
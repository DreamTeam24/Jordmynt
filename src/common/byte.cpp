#include "byte.h"
#include "error.h"
// ----------------------------------------------------------------------------
namespace jm {
// ----------------------------------------------------------------------------
BytesSpan defineVariable(BytesSpan& buffer,
                         BytesSize  variableSize)
{
    if (variableSize > buffer.size())
        throw Error("Buffer size is insufficient");
    
    BytesSpan variable{ buffer.data(), variableSize };
    buffer = buffer.last(buffer.size() - variableSize);
    return variable;
}
// ----------------------------------------------------------------------------
} // namespace jm
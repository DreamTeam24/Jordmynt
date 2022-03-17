#ifndef JM_COMMON_CONVERSIONS_H
#define JM_COMMON_CONVERSIONS_H
// ----------------------------------------------------------------------------
#include "byte.h"
#include <string>
#include <string_view>
// ----------------------------------------------------------------------------
namespace jm {
// ----------------------------------------------------------------------------
Bytes       convertHexStringToBytes(std::string_view hexString);
std::string convertBytesToHexString(BytesView bytes);
// ----------------------------------------------------------------------------
//BytesSpan   convertBase64StringToBytes(std::string_view base64String,
//                                       BytesSpan        destination);
//std::string convertBytesToBase64String(BytesView bytes);
// ----------------------------------------------------------------------------
//BytesSpan   convertTextStringToBytes(std::string_view textString,
//                                     BytesSpan        destination);
//std::string convertBytesToTextString(BytesView bytes);
// ----------------------------------------------------------------------------
//Bytes            convertSizeTo2Bytes(Bytes::size_type size);
//Bytes::size_type convert2BytesToSize(BytesView twoBytes);
// ----------------------------------------------------------------------------
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_COMMON_CONVERSIONS_H
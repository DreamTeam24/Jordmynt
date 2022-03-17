#include "conversions.h"
#include "error.h"
#include "logger/logger.h"
//#include <boost/archive/iterators/binary_from_base64.hpp>
//#include <boost/archive/iterators/base64_from_binary.hpp>
//#include <boost/archive/iterators/transform_width.hpp>
//#include <iomanip>
//#include <sstream>
#include <charconv>
// ----------------------------------------------------------------------------
namespace jm {
// ----------------------------------------------------------------------------
std::size_t constexpr charsPerByte{ 2 };
// ----------------------------------------------------------------------------
Bytes convertHexStringToBytes(std::string_view hexString)
{
    logger::Debug("Converting hexadecimal string to bytes ...");

    Bytes bytes(hexString.size() / charsPerByte);
    for (BytesSize i = 0; i < bytes.size(); ++i)
        if (std::from_chars(hexString.data() + i * charsPerByte,
                            hexString.data() + (i + 1) * charsPerByte,
                            bytes[i],
                            16).ec != std::errc())
            throw Error("Failed to convert hexadecimal string to bytes");

    return bytes;
}
// ----------------------------------------------------------------------------
std::string convertBytesToHexString(BytesView bytes)
{
    logger::Debug("Converting bytes to hexadecimal string ...");

    // std::to_chars does not write leading zeroes
    std::string hexString(bytes.size() * charsPerByte, '0');
    for (BytesSize i = 0; i < bytes.size(); ++i)
        if (std::to_chars(hexString.data() + i * charsPerByte + (bytes[i] < 0x10 ? 1 : 0),
                          hexString.data() + (i + 1) * charsPerByte,
                          bytes[i],
                          16).ec != std::errc())
            throw Error("Failed to convert bytes to hexadecimal string");

    return hexString;
}
// ----------------------------------------------------------------------------
//BytesSpan convertBase64StringToBytes(std::string_view base64String,
//                                     BytesSpan        destination)
//{
//    logger::Debug("Converting base64 string to bytes ...");
//
//    using iterator = boost::archive::iterators::transform_width<
//        boost::archive::iterators::binary_from_base64<
//        std::string_view::const_iterator >, 8, 6 >;
//
//    std::size_t padding = std::count(base64String.begin(), base64String.end(), '=');
//    Bytes bytes(iterator(base64String.begin()), iterator(base64String.end()));  // decode
//    bytes.erase(bytes.end() - padding, bytes.end());                            // erase padding '\0' characters
//
//    return bytes;
//}
//// ----------------------------------------------------------------------------
//std::string convertBytesToBase64String(BytesView bytes)
//{
//    logger::Debug("Converting bytes to base64 string ...");
//
//    using iterator = boost::archive::iterators::base64_from_binary<
//        boost::archive::iterators::transform_width<
//        BytesView::iterator, 6, 8 > >;
//
//    return std::string(iterator(bytes.begin()), iterator(bytes.end()))
//           .append((3 - bytes.size() % 3) % 3, '=');
//}
//// ----------------------------------------------------------------------------
//BytesSpan convertTextStringToBytes(std::string_view textString,
//                                   BytesSpan        destination)
//{
//    logger::Debug("Converting text string to bytes ..."); 
//    
//    return Bytes(textString.begin(), textString.end());
//}
//// ----------------------------------------------------------------------------
//std::string convertBytesToTextString(BytesView bytes)
//{
//    logger::Debug("Converting bytes to text string ..."); 
//    
//    return std::string(bytes.begin(), bytes.end());
//}
//// ----------------------------------------------------------------------------
//Bytes convertSizeTo2Bytes(Bytes::size_type size)
//{
//    logger::Debug("Converting size to bytes ...");
//
//    if (size < 256 * 256)
//        return Bytes{ static_cast<Byte>(size / 256), static_cast<Byte>(size % 256) };
//    else
//        throw std::runtime_error{ "Size exceeds 256^2." };
//}
//// ----------------------------------------------------------------------------
//Bytes::size_type convert2BytesToSize(BytesView twoBytes)
//{
//    logger::Debug("Converting bytes to size ...");
//
//    if(twoBytes.size() == 2)
//        return static_cast<Bytes::size_type>(twoBytes[0]) * 256 + static_cast<Bytes::size_type>(twoBytes[1]);
//    else
//        throw std::runtime_error{ "Number of bytes differs from 2." };
//}
// ----------------------------------------------------------------------------
} // namespace jm
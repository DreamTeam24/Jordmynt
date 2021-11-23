#include "conversions.h"

#include "logger/logger.h"
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <iomanip>
#include <sstream>

namespace jm {
namespace crypto {

Bytes convertHexStringToBytes(std::string const& hexString)
{
    // to do: check if valid hexadecimal string

    logger::DEBUG("Converting hexadecimal string to bytes ...");

    Bytes plain(hexString.size() / 2);
    for (std::string::size_type i = 0; i < hexString.size(); i += 2) {
        std::string byteString = hexString.substr(i, 2);
        plain[i / 2] = static_cast<unsigned char>(std::stoi(hexString.substr(i, 2).c_str(), nullptr, 16));
    }

    return plain;
}

std::string convertBytesToHexString(Bytes const& bytes)
{
    logger::DEBUG("Converting bytes to hexadecimal string ...");

    std::stringstream hexString;
    for (Bytes::size_type i = 0; i < bytes.size(); ++i)
        hexString << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(bytes[i]);

    return hexString.str();
}

Bytes convertBase64StringToBytes(std::string const& base64String)
{
    // to do: check if valid base64 string

    logger::DEBUG("Converting base64 string to bytes ...");

    using iterator = boost::archive::iterators::transform_width<
        boost::archive::iterators::binary_from_base64<
        std::string::const_iterator >, 8, 6 >;

    std::size_t padding = std::count(base64String.begin(), base64String.end(), '=');
    Bytes bytes(iterator(base64String.begin()), iterator(base64String.end()));  // decode
    bytes.erase(bytes.end() - padding, bytes.end());                            // erase padding '\0' characters

    return bytes;
}

std::string convertBytesToBase64String(Bytes const& bytes)
{
    logger::DEBUG("Converting bytes to base64 string ...");

    using iterator = boost::archive::iterators::base64_from_binary<
        boost::archive::iterators::transform_width<
        Bytes::const_iterator, 6, 8 > >;

    return std::string(iterator(bytes.begin()), iterator(bytes.end()))
        .append((3 - bytes.size() % 3) % 3, '=');
}

Bytes convertTextStringToBytes(std::string const& textString)
{
    logger::DEBUG("Converting text string to bytes ..."); 
    
    return Bytes(textString.begin(), textString.end());
}

std::string convertBytesToTextString(Bytes const& bytes)
{
    logger::DEBUG("Converting bytes to text string ..."); 
    
    return std::string(bytes.begin(), bytes.end());
}

} // namespace crypto
} // namespace jm
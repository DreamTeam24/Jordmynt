#ifndef JM_CRYPTO_CONVERT_H
#define JM_CRYPTO_CONVERT_H

#include <string>
#include <vector>

namespace jm {
namespace crypto {

using Bytes = std::vector<unsigned char>;

Bytes       convertHexStringToBytes(std::string const& hexString);
std::string convertBytesToHexString(Bytes const& bytes);

Bytes       convertBase64StringToBytes(std::string const& base64String);
std::string convertBytesToBase64String(Bytes const& bytes);

Bytes       convertTextStringToBytes(std::string const& textString);
std::string convertBytesToTextString(Bytes const& bytes);

} // namespace crypto
} // namespace jm

#endif // JM_CRYPTO_CONVERT_H
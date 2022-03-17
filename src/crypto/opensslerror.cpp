#include "opensslerror.h"
#include <openssl/err.h>
#include <string>
// ----------------------------------------------------------------------------
namespace jm {
namespace crypto {
// ----------------------------------------------------------------------------
OpenSSLError::OpenSSLError() :
    std::runtime_error{std::string("OpenSSL error: \n")
                       + ::ERR_reason_error_string(::ERR_get_error())}
{
}
// ----------------------------------------------------------------------------
} // namespace crypto
} // namespace jm
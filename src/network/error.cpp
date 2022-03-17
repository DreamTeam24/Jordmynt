#include "error.h"
#include <string>
// ----------------------------------------------------------------------------
namespace jm {
namespace network {
// ----------------------------------------------------------------------------
BoostAsioError::BoostAsioError(boost::system::error_code const& error) :
    std::runtime_error{std::string("Boost.Asio error: \n")
                       + error.message() }
{
}
// ----------------------------------------------------------------------------
} // namespace network
} // namespace jm
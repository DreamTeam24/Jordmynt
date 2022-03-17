#ifndef JM_NETWORK_ERROR_H
#define JM_NETWORK_ERROR_H
// ----------------------------------------------------------------------------
#include <boost/system/error_code.hpp>
#include <stdexcept>
// ----------------------------------------------------------------------------
namespace jm {
namespace network {
// ----------------------------------------------------------------------------
class BoostAsioError : public std::runtime_error
{
public:
    BoostAsioError(boost::system::error_code const& error);
};
// ----------------------------------------------------------------------------
} // namespace network
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_NETWORK_ERROR_H
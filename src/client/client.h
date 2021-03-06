#ifndef JM_CLIENT_CLIENT_H
#define JM_CLIENT_CLIENT_H
// ----------------------------------------------------------------------------
#include "common/byte.h"
#include <boost/asio.hpp>
#include <string>
// ----------------------------------------------------------------------------
namespace jm {
namespace client {
// ----------------------------------------------------------------------------
class Client
{
public:
    Client();

    void connect(std::string const& serverIPAddress,
                 std::string const& serverPort);
    void disconnect();

    void send(Bytes request);
    Bytes receive(Bytes::size_type nBytes);

private:
    boost::asio::io_context        m_ioContext; // m_resolver and m_socket depend on m_ioContext
    boost::asio::ip::tcp::resolver m_resolver;
    boost::asio::ip::tcp::socket   m_socket;
};
// ----------------------------------------------------------------------------
} // namespace client
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_CLIENT_CLIENT_H
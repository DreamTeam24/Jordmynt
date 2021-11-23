#ifndef JM_CLIENT_CLIENT_H
#define JM_CLIENT_CLIENT_H

#include <boost/asio.hpp>
#include <string>

namespace jm {
namespace client {

using Byte = unsigned char;
using Bytes = std::vector<Byte>;

class Client
{
public:
    Client();

    void connect(std::string const& serverIPAddress,
                 std::string const& serverPort);
    void disconnect();

    Bytes getResponse(Bytes const& requestToServer);

private:
    static constexpr Byte s_delimiter{ '\n' };
    
    boost::asio::io_context        m_ioContext; // m_resolver and m_socket depend on m_ioContext
    boost::asio::ip::tcp::resolver m_resolver;
    boost::asio::ip::tcp::socket   m_socket;
    Bytes                          m_buffer;
};

} // namespace client
} // namespace jm

#endif // JM_CLIENT_CLIENT_H
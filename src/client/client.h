#ifndef JM_CLIENT_CLIENT_H
#define JM_CLIENT_CLIENT_H

#include "app.h"
#include "crypto/bytes.h"
#include <boost/asio.hpp>
#include <memory>
#include <string>

namespace jm {
namespace client {

class Client
{
public:
    Client(boost::asio::io_context& ioContext,
           std::shared_ptr<App>     app);

    void connect(std::string const& server,
                 std::string const& service);
    void disconnect();

    void send();

private:
    void receiveHeader();
    void receiveBody(crypto::Bytes::size_type bodySize);

    static constexpr crypto::Bytes::size_type s_headerSize{ 2 };
    static constexpr crypto::Bytes::size_type s_maxBodySize{ 256 * 256 }; // 256^s_headersize
    
    boost::asio::ip::tcp::resolver m_resolver;
    boost::asio::ip::tcp::socket   m_socket;
    crypto::Bytes                  m_buffer;
    std::shared_ptr<App>           m_app;
};

} // namespace client
} // namespace jm

#endif // JM_CLIENT_CLIENT_H
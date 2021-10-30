#ifndef JM_SERVER_SERVER_H
#define JM_SERVER_SERVER_H

#include "crypto/bytes.h"
#include <boost/asio.hpp>
#include <memory>
#include <string>

namespace jm {
namespace server {

class Server
{
public:
    Server(boost::asio::io_context&   ioContext,
           boost::asio::ip::port_type port);

    void accept();

private:
    boost::asio::io_context&       m_ioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;

private:
    class Connection
        : public std::enable_shared_from_this<Connection>
    {
    public:
        Connection(boost::asio::ip::tcp::socket socket);

        void receiveHeader();

    private:
        static constexpr crypto::Bytes::size_type s_headerSize{ 2 };    // 2 bytes for data size

        void receiveMessage(crypto::Bytes::size_type messageSize);
        void sendResponse();

        boost::asio::ip::tcp::socket m_socket;
        crypto::Bytes                m_buffer;
    };
};

} // namespace server
} // namespace jm

#endif // JM_SERVER_SERVER_H
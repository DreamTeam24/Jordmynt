#include "client.h"

#include "logger/logger.h"

namespace jm {
namespace client {

Client::Client() :
    m_ioContext{},
    m_resolver{ m_ioContext },
    m_socket{ m_ioContext },
    m_buffer{}
{
}

void Client::connect(std::string const& serverIPAddress,
                     std::string const& serverPort)
{
    logger::INFO("Connecting to the server ...");
    
    boost::asio::connect(m_socket,
                         m_resolver.resolve(serverIPAddress, serverPort));
}

void Client::disconnect()
{
    logger::INFO("Disconnecting from the server ...");
    
    m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    m_socket.close();
}

Bytes Client::getResponse(Bytes const& requestToServer)
{
    logger::INFO("Sending request to the server ...");

    m_buffer = requestToServer;
    m_buffer.push_back(s_delimiter);
    boost::asio::write(m_socket,
                       boost::asio::dynamic_buffer(m_buffer));

    logger::INFO("Receiving response from the server ...");

    std::size_t responseSize{
        boost::asio::read_until(m_socket,
                                boost::asio::dynamic_buffer(m_buffer),
                                s_delimiter)
        - 1 };
    return Bytes{ m_buffer.begin(), m_buffer.begin() + responseSize };
}

} // namespace client
} // namespace jm
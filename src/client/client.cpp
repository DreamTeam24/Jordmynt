#include "client.h"
#include "logger/logger.h"
#include <stdexcept>
// ----------------------------------------------------------------------------
namespace jm {
namespace client {
// ----------------------------------------------------------------------------
Client::Client() :
    m_ioContext{},
    m_resolver{ m_ioContext },
    m_socket{ m_ioContext }
{
    logger::DEBUG("Creating a Client ...");
}
// ----------------------------------------------------------------------------
void Client::connect(std::string const& serverIPAddress,
                     std::string const& serverPort)
{
    logger::DEBUG("Connecting to the server ...");

    boost::system::error_code error;
    boost::asio::connect(m_socket,
                         m_resolver.resolve(serverIPAddress, serverPort),
                         error);

    if (error)
        throw std::runtime_error{ std::string("Boost.Asio error: \n")
                                  + error.message() };
}
// ----------------------------------------------------------------------------
void Client::disconnect()
{
    logger::DEBUG("Disconnecting from the server ...");

    m_socket.shutdown(boost::asio::socket_base::shutdown_both);
    m_socket.close();
}
// ----------------------------------------------------------------------------
void Client::send(Bytes request)
{
    logger::DEBUG("Sending request to the server ...");

    boost::system::error_code error;
    boost::asio::write(m_socket,
                       boost::asio::buffer(request, request.size()),
                       error);

    if (error)
        logger::THROW("Boost.Asio write failed\n" + error.message());
}
// ----------------------------------------------------------------------------
Bytes Client::receive(Bytes::size_type nBytes)
{
    logger::DEBUG("Receiving response from the server ...");

    Bytes response(nBytes);
    boost::system::error_code error;
    boost::asio::read(m_socket,
                      boost::asio::buffer(response, response.size()),
                      error);
    
    if (error == boost::asio::error::eof)
        disconnect();
    else if (error)
        logger::THROW("Boost.Asio read failed\n" + error.message());

    return response;
}
// ----------------------------------------------------------------------------
} // namespace client
} // namespace jm
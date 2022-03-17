#include "client.h"
#include "error.h"
#include "logger/logger.h"
#include "crypto/generaterandombytes.h"
#include <stdexcept>
// ----------------------------------------------------------------------------
namespace jm {
namespace network {
// ----------------------------------------------------------------------------
Client::Client() :
    m_ioContext{},
    m_socket{ m_ioContext },
    m_cipher{}
{
    logger::Debug("Creating a Client ...");
}
// ----------------------------------------------------------------------------
Client Client::connect(std::string_view serverIPAddress,
                       std::string_view serverPort,
                       std::string_view clientPrivateKeyFilename,
                       BytesView        serverPublicKey)
{
    logger::Debug("Connecting to the server ...");

    boost::asio::io_context ioContext{};
    boost::asio::ip::tcp::resolver resolver{ ioContext };
    boost::asio::ip::tcp::socket socket{ ioContext };
    boost::asio::connect(socket,
                         resolver.resolve(serverIPAddress, serverPort));
    
    Bytes memory(402);
    BytesSpan buffer{ memory };
    
    
    
    // Says hello
    boost::asio::write(socket,
                       boost::asio::buffer());
    
    
    
    if (error)
        throw BoostAsioError{ error };



    boost::system::system_error

    try
    {
        m_client.send(m_protocol.makeClientHello());
        Bytes serverHelloAndAuthenticationHeader{
            m_client.receive(m_protocol.getHeaderSize()) };
        Bytes serverHelloAndAuthenticationBody{
            m_client.receive(m_protocol.readBodySize(serverHelloAndAuthenticationHeader)) };
        m_client.send(m_protocol.makeClientAuthentication(serverHelloAndAuthenticationHeader,
            serverHelloAndAuthenticationBody));
    }
    catch (Bytes const& alertRecord)
    {
        m_client.send(alertRecord);
        m_client.disconnect();
        throw;
    }

    */
}
// ----------------------------------------------------------------------------
void Client::disconnect()
{
    logger::Debug("Disconnecting from the server ...");

    try
    {
        boost::asio::write(m_socket,
                           boost::asio::buffer(m_protocol.makeDisconnectNotification()));
    }
    catch (std::exception const& exception)
    {
        closeSocket();
        throw;
    }
    closeSocket();
}
// ----------------------------------------------------------------------------
void Client::send(Bytes request)
{
    logger::Debug("Sending request to the server ...");

    try
    {
        m_cipher.encryptAndAuthenticate(request, )
        
        boost::asio::write(m_socket,
                           boost::asio::buffer(m_protocol.encrypt(request)));
    }
    catch (std::exception const& exception)
    {
        closeSocket();
        throw;
    }
}
// ----------------------------------------------------------------------------
Bytes Client::receive()
{
    logger::Debug("Receiving response from the server ...");

    try
    {
        Bytes header(m_protocol.getHeaderSize());
        boost::asio::read(m_socket,
                          boost::asio::buffer(header));
    
        Bytes body(m_protocol.readBodySize(header));
        boost::asio::read(m_socket,
                          boost::asio::buffer(body));

        return m_protocol.decrypt(header, body);
    }
    catch (crypto::Protocol::Alert alert)
    {
        try
        {
            boost::asio::write(m_socket,
                               boost::asio::buffer(alert.get()));
        }
        catch (std::exception const& exception)
        {
            closeSocket();
            throw;
        }
        closeSocket();
        throw;
    }
    catch (std::exception const& exception)
    {
        closeSocket();
        throw;
    }
}
// ----------------------------------------------------------------------------
void Client::closeSocket()
{
    m_socket.shutdown(boost::asio::socket_base::shutdown_both);
    m_socket.close();
}
// ----------------------------------------------------------------------------
} // namespace network
} // namespace jm
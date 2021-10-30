#include "client.h"
#include "logger/logger.h"
#include <cassert>

namespace jm {
namespace client {

// The memory address of m_buffer must never change.
// Therefore, m_buffer is sized to its maximum size in the constructor.

Client::Client(boost::asio::io_context& ioContext,
               std::shared_ptr<App>     app) :
    m_resolver{ ioContext },
    m_socket{ ioContext },
    m_buffer(s_headerSize + s_maxBodySize), // {}-initialization does not set the size
    m_app{ app }
{
}

void Client::connect(std::string const& server,
                     std::string const& service)
{
    logger::INFO("Connecting to the server ...");
    
    boost::asio::async_connect(
        m_socket,
        m_resolver.resolve(server, service),
        [this](boost::system::error_code const&      error,
               boost::asio::ip::tcp::endpoint const& endpoint)
        {
            if (!error)
            {
                this->m_app->start();
                this->receiveHeader();
            }
            else
            {
                logger::WARNING("Failed to connect to the server: " + error.message());
            }
        }
    );
}

void Client::disconnect()
{
    logger::INFO("Disconnecting from the server ...");

    this->m_app->stop();
    m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_type::shutdown_both);
    m_socket.close();
}

void Client::send()
{
    logger::INFO("Sending message to the server ...");

    crypto::Bytes message{ m_app->getMessageForServer() };
    assert(message.size() <= s_maxBodySize);

    m_buffer[0] = static_cast<crypto::Byte>(message.size()) / 256;              // header
    m_buffer[1] = static_cast<crypto::Byte>(message.size()) % 256;              // header
    std::copy(message.begin(), message.end(), std::next(m_buffer.begin(), 2));  // message
        
    boost::asio::async_write(
        m_socket,
        boost::asio::buffer(m_buffer, 2 + message.size()),
        [this](boost::system::error_code const& error,
                std::size_t                    /*size*/)
        {
            if (error)
            {
                this->m_app->stop();
                if (error != boost::asio::error::eof)
                    logger::WARNING("Failed to send message to the server: " + error.message());
            }
        }
    );
}

void Client::receiveHeader()
{
    logger::INFO("Receiving header from the server ...");

    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(m_buffer, s_headerSize),
        [this](boost::system::error_code const& error,
               std::size_t                    /*size*/)
        {
            if (!error)
            {
                crypto::Bytes::size_type bodySize{ static_cast<crypto::Bytes::size_type>(m_buffer[0]) * 256 + 
                                                   static_cast<crypto::Bytes::size_type>(m_buffer[1]) };
                this->receiveBody(bodySize);
            }
            else
            {
                this->m_app->stop();
                if (error != boost::asio::error::eof)
                    logger::WARNING("Failed to receive header from the server: " + error.message());
            }
        }
    );
}

void Client::receiveBody(crypto::Bytes::size_type bodySize)
{
    logger::INFO("Receiving response from the server ...");

    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(m_buffer, bodySize),
        [this](boost::system::error_code const& error,
               std::size_t                    /*size*/)
        {
            if (!error)
            {
                this->m_app->setResponseFromServer(this->m_buffer);
                this->receiveHeader();
            }
            else
            {
                this->m_app->stop();
                if (error != boost::asio::error::eof)
                    logger::WARNING("Failed to receive response from the server: " + error.message());
            }
        }
    );
}

} // namespace client
} // namespace jm
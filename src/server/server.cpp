#include "server.h"
#include "crypto/conversions.h"
#include "logger/logger.h"
// ----------------------------------------------------------------------------
namespace jm {
namespace server {
// ----------------------------------------------------------------------------
Server::Server(boost::asio::io_context&   ioContext,
               boost::asio::ip::port_type port) :
    m_ioContext{ ioContext },
    m_acceptor{ ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) }
{
}
// ----------------------------------------------------------------------------
void Server::accept()
{
    logger::INFO("Accepting a client ...");

    m_acceptor.async_accept(
        [this](boost::system::error_code const& error,
               boost::asio::ip::tcp::socket     socket)
        {
            if (error)
                logger::WARNING("Failed to accept the client: " + error.message());
            else
                std::make_shared<Connection>(std::move(socket))->receiveHeader();

            this->accept();     // accept another client
        }
    );
}
// ----------------------------------------------------------------------------
Server::Connection::Connection(boost::asio::ip::tcp::socket socket) :
    m_socket{ std::move(socket) },
    m_buffer{}
{
    logger::INFO("Connection established");
}
// ----------------------------------------------------------------------------
void Server::Connection::receiveHeader()
{
    logger::INFO("Receiving header from the client ...");

    m_buffer.resize(s_headerSize);

    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(m_buffer, m_buffer.size()),
        [this2 = shared_from_this()](boost::system::error_code const& error,
                                     std::size_t                    /*size*/)
        {
            if (!error)
            {
                crypto::Bytes::size_type messageSize{ static_cast<crypto::Bytes::size_type>(this2->m_buffer[0]) * 256 + static_cast<crypto::Bytes::size_type>(this2->m_buffer[1]) };
                this2->receiveMessage(messageSize);
            }
            else
            {
                if (error != boost::asio::error::eof)
                    logger::WARNING("Failed to receive header from the client: " + error.message());
            }
        }
    );
}
// ----------------------------------------------------------------------------
void Server::Connection::receiveMessage(crypto::Bytes::size_type messageSize)
{
    logger::INFO("Receiving message from the client ...");

    m_buffer.resize(messageSize);

    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(m_buffer, m_buffer.size()),
        [this2 = shared_from_this()](boost::system::error_code const& error,
                                     std::size_t                    /*size*/)
        {
            if (!error)
                this2->sendResponse();
            else
            {
                if (error != boost::asio::error::eof)
                    logger::WARNING("Failed to receive message from the client: " + error.message());
            }
        }
    );
}
// ----------------------------------------------------------------------------
void Server::Connection::sendResponse()
{
    logger::INFO("Sending response to the client ...");

    crypto::Bytes message{ m_buffer };

    m_buffer.resize(2 + message.size() + 5);
    m_buffer[0] = static_cast<crypto::Byte>(message.size() + 5) / 256;          // header
    m_buffer[1] = static_cast<crypto::Byte>(message.size() + 5) % 256;          // header
    std::copy(message.begin(), message.end(), std::next(m_buffer.begin(), 2));  // response
    crypto::Bytes suffix{ crypto::convertTextStringToBytes(" too!") };
    std::copy(suffix.begin(), suffix.end(), std::next(m_buffer.end(), -5));

    boost::asio::async_write(
        m_socket,
        boost::asio::buffer(m_buffer, m_buffer.size()),
        [this2 = shared_from_this()](boost::system::error_code const& error,
                                     std::size_t                    /*size*/)
        {
            if (!error)
                this2->receiveHeader();
            else
            {
                if (error != boost::asio::error::eof)
                    logger::WARNING("Failed to send response to the client: " + error.message());
            }
        }
    );
}
// ----------------------------------------------------------------------------
} // namespace server
} // namespace jm
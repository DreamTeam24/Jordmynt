#ifndef JM_NETWORK_CLIENT_H
#define JM_NETWORK_CLIENT_H
// ----------------------------------------------------------------------------
#include "common/byte.h"
#include "crypto/cipher.h"
#include <boost/asio.hpp>
#include <string_view>
// ----------------------------------------------------------------------------
namespace jm {
namespace network {
// ----------------------------------------------------------------------------
class Client
{
public:
    ~Client();

    static Client connect(std::string_view serverIPAddress,
                          std::string_view serverPort,
                          std::string_view clientPrivateKeyFilename,
                          BytesView        serverPublicKey);
    void disconnect();

    void send(Bytes request);
    Bytes receive();

private:
    Client();

    void closeSocket();

    BytesSize const s_randomBytesSize;

    boost::asio::io_context      m_ioContext; // m_resolver and m_socket depend on m_ioContext
    boost::asio::ip::tcp::socket m_socket;
    crypto::Cipher               m_cipher;
};
// ----------------------------------------------------------------------------
} // namespace network
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_NETWORK_CLIENT_H
#ifndef JM_SECURECLIENT_SECURECLIENT_H
#define JM_SECURECLIENT_SECURECLIENT_H
// ----------------------------------------------------------------------------
#include "common/byte.h"
#include "client/client.h"
#include "crypto/protocol.h"
#include <string>
// ----------------------------------------------------------------------------
namespace jm {
namespace secureclient {
// ----------------------------------------------------------------------------
class SecureClient
{
public:
    SecureClient();

    void connect(std::string const& serverIPAddress,
                 std::string const& serverPort,
                 std::string const& clientPrivateIdentityKeyFilename,
                 Bytes const&       serverPublicIdentityKey);
    void disconnect();

    void send(Bytes const& request);
    Bytes receive();

private:
    client::Client   m_client;
    crypto::Protocol m_protocol;
};
// ----------------------------------------------------------------------------
} // namespace secureclient
} // namespace jm
// ----------------------------------------------------------------------------
#endif // JM_SECURECLIENT_SECURECLIENT_H
#include "secureclient.h"
#include "logger/logger.h"
#include "crypto/decrypter.h"
#include "crypto/encrypter.h"
#include <algorithm>
#include <utility>
// ----------------------------------------------------------------------------
namespace jm {
namespace secureclient {
// ----------------------------------------------------------------------------
SecureClient::SecureClient() :
    m_client{},
    m_protocol{}
{
    logger::DEBUG("Creating a SecureClient ...");
}
// ----------------------------------------------------------------------------
void SecureClient::connect(std::string const& serverIPAddress,
                           std::string const& serverPort,
                           std::string const& clientPrivateIdentityKeyFilename,
                           Bytes const&       serverPublicIdentityKey)
{
    m_client.connect(serverIPAddress, serverPort);

    logger::DEBUG("Securing connection ...");

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
}
// ----------------------------------------------------------------------------
void SecureClient::disconnect()
{
    logger::DEBUG("Notifying server of disconnecting ...");
    
    m_client.send(m_protocol.makeDisconnectNotification());
    m_client.disconnect();
}
// ----------------------------------------------------------------------------
void SecureClient::send(Bytes const& request)
{
    m_client.send(m_protocol.encrypt(request));
}
// ----------------------------------------------------------------------------
Bytes SecureClient::receive()
{
    try
    {
        Bytes header{ m_client.receive(m_protocol.getHeaderSize()) };
        Bytes body{ m_client.receive(m_protocol.readBodySize(header)) };
        return m_protocol.decrypt(header, body);
    }
    catch (Bytes const& alertRecord)
    {
        m_client.send(alertRecord);
        m_client.disconnect();
        throw;
    }
}
// ----------------------------------------------------------------------------
} // namespace secureclient
} // namespace jm
#ifndef JM_CLIENT_APP_H
#define JM_CLIENT_APP_H

#include <memory>
#include <string>

namespace jm {
namespace client {

class App
{
public:
    App();

    virtual void connect(std::string const& serverIPAddress,
                         std::string const& serverPort) = 0;
    virtual void sendRequestToServer(std::string request) = 0;
    virtual std::string receiveResponseFromServer() = 0;
    virtual void disconnect() = 0;

protected:
    friend class Client;
    
    virtual void handleConnected(std::string const& serverIPAddress,
                                 std::string const& serverPort) = 0;
    virtual void handleConnectFailed(std::string const& errorMessage) = 0;

    virtual void handleDisconnected() = 0;

    virtual std::string getRequest() const = 0;
    virtual void handleSent(std::size_t nBytesSent) = 0;
    virtual void handleSendFailed(std::string const& errorMessage) = 0;

    virtual void handleReceived(std::string response) = 0;
    virtual void handleReceiveFailed(std::string const& errorMessage) = 0;

    std::unique_ptr<Client> m_client;
};

} // namespace client
} // namespace jm

#endif // JM_CLIENT_APP_H
#ifndef JM_CLIENTAPP_CLIENTAPP_H
#define JM_CLIENTAPP_CLIENTAPP_H

#include "client/app.h"
//#include "client/client.h"
//#include <boost/asio.hpp>
//#include <wx/wxprec.h>
//#ifndef WX_PRECOMP
//   #include <wx/wx.h>
//#endif
#include <condition_variable>
#include <mutex>

namespace jm {
namespace clientapp {

class LoginDialog;
class GUI;

class ClientApp : public client::App
{
public:
    ClientApp(LoginDialog const& loginDialog);
    
    void sendRequestToServer(std::string request) override;
    std::string receiveResponseFromServer() override;
                                                                
private:
    void handleConnected(std::string const& serverIPAddress,
        std::string const& serverPort) override;
    void handleConnectFailed(std::string const& errorMessage) override;

    void handleDisconnected() override;

    std::string getRequest() const override;
    void handleSent(std::size_t nBytesSent) override;
    void handleSendFailed(std::string const& errorMessage) override;

    void handleReceived(std::string response) override;
    void handleReceiveFailed(std::string const& errorMessage) override;

    
    //std::mutex mutable      m_mutex;
    //std::condition_variable m_condition;
    //GUI*                    m_gui;
    //crypto::Bytes           m_clientData;
    //crypto::Bytes           m_serverData;
    //bool                    m_isClientDataSet;
    //bool                    m_isServerDataSet;
    //bool                    m_connected;



    //boost::asio::io_context m_ioContext;
    //client::Client          m_client; // must come after m_ioContext and m_app

};

} // namespace clientapp
} // namespace jm

#endif // JM_CLIENTAPP_CLIENTAPP_H

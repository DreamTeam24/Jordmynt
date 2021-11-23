#include "clientapp.h"

#include "client/client.h"
#include "logindialog.h" 
#include "gui.h" 
#include <thread>

//#include "id.h"
//#include "crypto/functions.h"

namespace jm {
namespace clientapp {

ClientApp::ClientApp(LoginDialog const& loginDialog) :
    App{}
    //m_ioContext{},
    //m_client{ m_ioContext, std::make_shared<ClientApp>(this) },
    //m_gui{ loginDialog.getGUI() },
    //m_mutex{},
    //m_condition{},
    //m_clientData{},
    //m_serverData{},
    //m_isClientDataSet{ false },
    //m_isServerDataSet{ false },
    //m_connected{ false }
{
    m_client->connect()
    
    //m_client.connect(loginDialog.getServerIPAddress(), loginDialog.getServerPort());
    //std::thread networkCommunication
    //{
    //    [this](){ m_ioContext.run(); }
    //};
    //networkCommunication.detach();
}

void ClientApp::sendRequestToServer(std::string request)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this] {return (this->m_isServerDataSet == false) ||
        (this->m_isStopped == true); });

    if (m_isServerDataSet == false)
    {
        m_serverData = std::move(message);
        m_isServerDataSet = true;
    }
    else
        m_serverData.clear();

    lock.unlock();
    m_condition.notify_one();
}

std::string ClientApp::receiveResponseFromServer()
{

}

void ClientApp::handleConnected(std::string const& serverIPAddress,
    std::string const& serverPort)
{

}
void ClientApp::handleConnectFailed(std::string const& errorMessage)
{

}

void ClientApp::handleDisconnected()
{

}

std::string ClientApp::getRequest() const
{

}
void ClientApp::handleSent(std::size_t nBytesSent)
{

}
void ClientApp::handleSendFailed(std::string const& errorMessage)
{

}

void ClientApp::handleReceived(std::string response)
{

}
void ClientApp::handleReceiveFailed(std::string const& errorMessage)
{

}












void ClientApp::setConnected(bool connected)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connected = connected;
    }
    wxThreadEvent event(wxEVT_THREAD, GUI_Connected);
    m_gui->QueueEvent(new wxThreadEvent{ wxEVT_THREAD, GUI_Connected });
    
    wxQueueEvent(m_gui, event.Clone());
}

void App::stop()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_isStopped = true;
    wxThreadEvent event(wxEVT_THREAD, ID_Stop);
    wxQueueEvent(m_loginFrame, event.Clone());
}

void App::setMessageForServer(crypto::Bytes message)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this] {return (this->m_isServerDataSet == false) ||
                                          (this->m_isStopped == true); });

    if (m_isServerDataSet == false)
    {
        m_serverData = std::move(message);
        m_isServerDataSet = true;
    }
    else
        m_serverData.clear();
    
    lock.unlock();
    m_condition.notify_one();
}

crypto::Bytes App::getMessageForServer()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this] {return (this->m_isServerDataSet == true) ||
                                          (this->m_isStopped == true); });
    
    crypto::Bytes serverData{ m_serverData };
    if (m_isServerDataSet == true)
        m_isServerDataSet = false;
    
    lock.unlock();
    m_condition.notify_one();

    return serverData; // return a copy of m_serverData to avoid a data race
}

void App::setResponseFromServer(crypto::Bytes response)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this] {return (this->m_isClientDataSet == false) ||
                                          (this->m_isStopped == true); });

    if (m_isClientDataSet == false)
    {
        m_clientData = std::move(response);
        m_isClientDataSet = true;
        wxThreadEvent event(wxEVT_THREAD, ID_Receive);
        wxQueueEvent(m_loginFrame, event.Clone());
    }
    else
        m_clientData.clear();

    lock.unlock();
    m_condition.notify_one();
}

crypto::Bytes App::getResponseFromServer()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this] {return (this->m_isClientDataSet == true) ||
                                          (this->m_isStopped == true); });

    crypto::Bytes clientData{ m_clientData };
    if (m_isClientDataSet == true)
        m_isClientDataSet = false;

    lock.unlock();
    m_condition.notify_one();

    return clientData; // return a copy of m_clientData to avoid a data race
}

} // namespace clientapp
} // namespace jm
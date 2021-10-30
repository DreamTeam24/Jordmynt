#include "mainframe.h" 
#include "app.h"
#include "id.h"
#include "crypto/functions.h"

namespace jm {
namespace clientapp {

App::App(MainFrame* loginFrame) :
    m_loginFrame{ loginFrame },
    m_mutex{},
    m_condition{},
    m_clientData{},
    m_serverData{},
    m_isClientDataSet{ false },
    m_isServerDataSet{ false },
    m_isStopped{ true }
{
}

void App::start()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_clientData.clear();
    m_serverData.clear();
    m_isClientDataSet = false;
    m_isServerDataSet = false;
    m_isStopped = false;
    wxThreadEvent event(wxEVT_THREAD, ID_Start);
    wxQueueEvent(m_loginFrame, event.Clone());
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
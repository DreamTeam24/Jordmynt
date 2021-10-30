#ifndef JM_CLIENTAPP_APP_H
#define JM_CLIENTAPP_APP_H

#include "crypto/bytes.h"
#include "client/app.h"
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <condition_variable>
#include <mutex>

namespace jm {
namespace clientapp {

class MainFrame;

class App : public client::App
{
public:
    App(MainFrame* loginFrame);
    
    virtual void start();
    virtual void stop();

    void setMessageForServer(crypto::Bytes message);
    virtual crypto::Bytes getMessageForServer();
    virtual void setResponseFromServer(crypto::Bytes response);
    crypto::Bytes getResponseFromServer();
                                                                
private:
    MainFrame*             m_loginFrame;  // std::shared_ptr doesn't work
    std::mutex              m_mutex;
    std::condition_variable m_condition;
    crypto::Bytes           m_clientData;
    crypto::Bytes           m_serverData;
    bool                    m_isClientDataSet;
    bool                    m_isServerDataSet;
    bool                    m_isStopped;
};

} // namespace clientapp
} // namespace jm

#endif // JM_CLIENTAPP_APP_H

#ifndef JM_CLIENTAPP_MAINFRAME_H
#define JM_CLIENTAPP_MAINFRAME_H

#include "client/client.h"
#include <boost/asio.hpp>
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <memory>

namespace jm {
namespace clientapp {

class App;

class MainFrame : public wxFrame
{
public:
    MainFrame(wxString const& title);

    void handleClose(wxCloseEvent& event);
    void handleExit(wxCommandEvent& event);

    //void handleAbout(wxCommandEvent& event);
    //void handleSetup(wxCommandEvent& event);
    //void handleConnect(wxCommandEvent& event);
    //void handleSend(wxCommandEvent& event);
    //void handleDisconnect(wxCommandEvent& event);
    //void handleStart(wxThreadEvent& event);
    //void handleReceive(wxThreadEvent& event);
    //void handleStop(wxThreadEvent& event);
    //void popUp(wxCommandEvent& event);

private:
    boost::asio::io_context m_ioContext;
    std::shared_ptr<App>    m_app; 
    client::Client          m_client; // must come after m_ioContext and m_app

    wxDECLARE_EVENT_TABLE();
};

} // namespace clientapp
} // namespace jm

#endif // JM_CLIENTAPP_MAINFRAME_H
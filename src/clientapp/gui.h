#ifndef JM_CLIENTAPP_GUI_H
#define JM_CLIENTAPP_GUI_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <memory>

namespace jm {
namespace clientapp {

enum
{
    GUI_LogIn,
    GUI_Connected,
    GUI_LogOut
};

class ClientApp;

class GUI : public wxFrame
{
public:
    GUI(wxString const& title);

    void handleClose(wxCloseEvent& event);
    void handleExit(wxCommandEvent& event);

    void handleLogIn(wxCommandEvent& event);
    void handleLogOut(wxCommandEvent& event);

    void handleConnected(wxCommandEvent& event);

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
    // afgeleide klasse van Client
    
    
    std::shared_ptr<ClientApp> m_clientApp;

    wxDECLARE_EVENT_TABLE();
};

} // namespace clientapp
} // namespace jm

#endif // JM_CLIENTAPP_GUI_H
#ifndef JM_CLIENTAPP_LOGINDIALOG_H
#define JM_CLIENTAPP_LOGINDIALOG_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <string>

namespace jm {
namespace clientapp {

class GUI;

class LoginDialog : public wxDialog
{
public:
    LoginDialog(GUI* gui);

    GUI* getGUI() const;
    std::string getClientAccountNumber() const;
    std::string getClientPassword() const;
    std::string getClientPrivateKeyFilename() const;
    std::string getServerIPAddress() const;
    std::string getServerPort() const;
    std::string getServerPublicKeyFilename() const;

private:
    void handleClose(wxCloseEvent& event);
    void handleBrowseClientPrivateKeyFile(wxCommandEvent& event);
    void handleBrowseServerPublicKeyFile(wxCommandEvent& event);
    void handleLogIn(wxCommandEvent& event);
    void handleCancel(wxCommandEvent& event);
    
    GUI*          m_gui;

    wxString      m_clientAccountNumber;
    wxStaticText* m_clientAccountNumberLabel;
    wxTextCtrl*   m_clientAccountNumberTextbox;       // must come after m_clientAccountNumber for validator

    wxString      m_clientPassword;
    wxStaticText* m_clientPasswordLabel;
    wxTextCtrl*   m_clientPasswordTextbox;

    wxString      m_clientPrivateKeyFilename;
    wxStaticText* m_clientPrivateKeyFilenameLabel;
    wxTextCtrl*   m_clientPrivateKeyFilenameTextbox;  // must come after m_clientPrivateKeyFilename for validator
    wxButton*     m_clientPrivateKeyFilenameButton;
    
    wxString      m_serverIPAddress;
    wxStaticText* m_serverIPAddressLabel;
    wxTextCtrl*   m_serverIPAddressTextbox;           // must come after m_serverIPAddress for validator

    wxString      m_serverPort;
    wxStaticText* m_serverPortLabel;
    wxTextCtrl*   m_serverPortTextbox;                // must come after m_serverPort for validator

    wxString      m_serverPublicKeyFilename;
    wxStaticText* m_serverPublicKeyFilenameLabel;
    wxTextCtrl*   m_serverPublicKeyFilenameTextbox;   // must come after m_serverPublicKeyFilename for validator
    wxButton*     m_serverPublicKeyFilenameButton;

    wxButton*     m_loginButton;
    wxButton*     m_cancelButton;

    class ClientAccountNumberValidator : public wxTextValidator
    {
    public:
        ClientAccountNumberValidator(wxString* value);

        virtual wxObject* Clone() const wxOVERRIDE;
        virtual bool Validate(wxWindow* parent) wxOVERRIDE;

    private:
        wxString const m_errorCaption;
        wxString const m_errorMessage;
    };

    class NonEmptyValidator : public wxTextValidator
    {
    public:
        NonEmptyValidator(wxString*       value,
                          wxString const& errorCaption,
                          wxString const& errorMessage);

        virtual wxObject* Clone() const wxOVERRIDE;
        virtual bool Validate(wxWindow* parent) wxOVERRIDE;

    private:
        wxString m_errorCaption;
        wxString m_errorMessage;
    };

    wxDECLARE_EVENT_TABLE();
};

} // namespace clientapp
} // namespace jm

#endif // JM_CLIENTAPP_LOGINDIALOG_H

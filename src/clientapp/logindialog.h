#ifndef JM_CLIENTAPP_LOGINDIALOG_H
#define JM_CLIENTAPP_LOGINDIALOG_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

namespace jm {
namespace clientapp {

class LoginDialog : public wxDialog
{
public:
    LoginDialog(wxFrame* mainFrame);

    wxString getUserAccountNumber() const;
    wxString getUserPassword() const;
    wxString getUserPrivateKeyFilename() const;
    wxString getBankIPAddress() const;
    wxString getBankPort() const;
    wxString getBankPublicKeyFilename() const;

private:
    void handleClose(wxCloseEvent& event);
    void handleBrowseUserPrivateKeyFile(wxCommandEvent& event);
    void handleBrowseBankPublicKeyFile(wxCommandEvent& event);
    void handleLogIn(wxCommandEvent& event);
    void handleCancel(wxCommandEvent& event);
    
    wxString      m_userAccountNumber;
    wxStaticText* m_userAccountNumberLabel;
    wxTextCtrl*   m_userAccountNumberTextbox;       // must come after m_userAccountNumber for validator

    wxString      m_userPassword;
    wxStaticText* m_userPasswordLabel;
    wxTextCtrl*   m_userPasswordTextbox;

    wxString      m_userPrivateKeyFilename;
    wxStaticText* m_userPrivateKeyFilenameLabel;
    wxTextCtrl*   m_userPrivateKeyFilenameTextbox;  // must come after m_userPrivateKeyFilename for validator
    wxButton*     m_userPrivateKeyFilenameButton;
    
    wxString      m_bankIPAddress;
    wxStaticText* m_bankIPAddressLabel;
    wxTextCtrl*   m_bankIPAddressTextbox;           // must come after m_bankIPAddress for validator

    wxString      m_bankPort;
    wxStaticText* m_bankPortLabel;
    wxTextCtrl*   m_bankPortTextbox;                // must come after m_bankPort for validator

    wxString      m_bankPublicKeyFilename;
    wxStaticText* m_bankPublicKeyFilenameLabel;
    wxTextCtrl*   m_bankPublicKeyFilenameTextbox;   // must come after m_bankPublicKeyFilename for validator
    wxButton*     m_bankPublicKeyFilenameButton;

    wxButton*     m_loginButton;
    wxButton*     m_cancelButton;

    class UserAccountNumberValidator : public wxTextValidator
    {
    public:
        UserAccountNumberValidator(wxString* value);

        virtual wxObject* Clone() const wxOVERRIDE;
        virtual bool Validate(wxWindow* parent) wxOVERRIDE;

    private:
        wxString const m_errorCaption;
        wxString const m_errorMessage;
    };

    class NonEmptyValidator : public wxTextValidator
    {
    public:
        NonEmptyValidator(wxString* value,
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

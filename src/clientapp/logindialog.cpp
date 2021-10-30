#include "logindialog.h"
#include "errorbox.h"
#include <wx/statline.h>
#include <wx/stdpaths.h>
#include <algorithm>

namespace jm {
namespace clientapp {

enum
{
    LOGIN_BrowseUserPrivateKeyFile = 1,
    LOGIN_BrowseBankPublicKeyFile = 2,
    LOGIN_LogIn = wxID_OK,
    LOGIN_Cancel = wxID_CANCEL
};

wxBEGIN_EVENT_TABLE(LoginDialog, wxDialog)
    EVT_CLOSE(LoginDialog::handleClose)
    EVT_BUTTON(LOGIN_BrowseUserPrivateKeyFile, LoginDialog::handleBrowseUserPrivateKeyFile)
    EVT_BUTTON(LOGIN_BrowseBankPublicKeyFile, LoginDialog::handleBrowseBankPublicKeyFile)
    EVT_BUTTON(LOGIN_LogIn, LoginDialog::handleLogIn)
    EVT_BUTTON(LOGIN_Cancel, LoginDialog::handleCancel)
wxEND_EVENT_TABLE()

LoginDialog::LoginDialog(wxFrame* mainFrame) :
    wxDialog{ mainFrame, wxID_ANY, "Log in", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE },
    m_userAccountNumber{},
    m_userAccountNumberLabel{ new wxStaticText(this, wxID_ANY, "My account number") },
    m_userAccountNumberTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER,
                                UserAccountNumberValidator{ &m_userAccountNumber }) },
    m_userPassword{},
    m_userPasswordLabel{ new wxStaticText(this, wxID_ANY, "My password") },
    m_userPasswordTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD) },
    m_userPrivateKeyFilename{},
    m_userPrivateKeyFilenameLabel{ new wxStaticText(this, wxID_ANY, "My private key file") },
    m_userPrivateKeyFilenameTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY,
                                     NonEmptyValidator{ &m_userPrivateKeyFilename, "Invalid private key file", "Please select your private key file."})},
    m_userPrivateKeyFilenameButton{ new wxButton(this, LOGIN_BrowseUserPrivateKeyFile, "Browse...") },
    m_bankIPAddress{},
    m_bankIPAddressLabel{ new wxStaticText(this, wxID_ANY, "Bank IP address or domain name") },
    m_bankIPAddressTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER,
                            NonEmptyValidator{ &m_bankIPAddress, "Invalid IP address or domain name", "Please enter the bank's IP address or domain name." }) },
    m_bankPort{},
    m_bankPortLabel{ new wxStaticText(this, wxID_ANY, "Bank port or service name") },
    m_bankPortTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER,
                       NonEmptyValidator{ &m_bankPort, "Invalid port or service name", "Please enter the bank's port or service name." }) },
    m_bankPublicKeyFilename{},
    m_bankPublicKeyFilenameLabel{ new wxStaticText(this, wxID_ANY, "Bank public key file") },
    m_bankPublicKeyFilenameTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY,
                                    NonEmptyValidator{ &m_bankPublicKeyFilename, "Invalid public key file", "Please select the bank's public key file." })},
    m_bankPublicKeyFilenameButton{ new wxButton(this, LOGIN_BrowseBankPublicKeyFile, "Browse...") },
    m_loginButton{ new wxButton(this, LOGIN_LogIn , "Log in") },
    m_cancelButton{ new wxButton(this, LOGIN_Cancel, "Cancel") }
{
    m_userAccountNumberTextbox->SetHint("0000 0000 0000");

    wxBoxSizer* userPrivateKeyFilenameSizer = new wxBoxSizer(wxHORIZONTAL);
    userPrivateKeyFilenameSizer->Add(m_userPrivateKeyFilenameTextbox, wxSizerFlags(1));
    userPrivateKeyFilenameSizer->AddSpacer(10);
    userPrivateKeyFilenameSizer->Add(m_userPrivateKeyFilenameButton);

    wxBoxSizer* bankPublicKeyFilenameSizer = new wxBoxSizer(wxHORIZONTAL);
    bankPublicKeyFilenameSizer->Add(m_bankPublicKeyFilenameTextbox, wxSizerFlags(1));
    bankPublicKeyFilenameSizer->AddSpacer(10);
    bankPublicKeyFilenameSizer->Add(m_bankPublicKeyFilenameButton);

    wxBoxSizer* buttonInnerSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonInnerSizer->Add(m_loginButton);
    buttonInnerSizer->AddSpacer(10);
    buttonInnerSizer->Add(m_cancelButton);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(buttonInnerSizer, wxSizerFlags().Right());
    
    wxBoxSizer* loginInnerSizer = new wxBoxSizer(wxVERTICAL);
    loginInnerSizer->Add(m_userAccountNumberLabel);
    loginInnerSizer->Add(m_userAccountNumberTextbox, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(m_userPasswordLabel);
    loginInnerSizer->Add(m_userPasswordTextbox, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(m_userPrivateKeyFilenameLabel);
    loginInnerSizer->Add(userPrivateKeyFilenameSizer, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(15);
    loginInnerSizer->Add(new wxStaticLine(this), wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(m_bankIPAddressLabel);
    loginInnerSizer->Add(m_bankIPAddressTextbox, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(m_bankPortLabel);
    loginInnerSizer->Add(m_bankPortTextbox, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(m_bankPublicKeyFilenameLabel);
    loginInnerSizer->Add(bankPublicKeyFilenameSizer, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(15);
    loginInnerSizer->Add(new wxStaticLine(this), wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(buttonSizer, wxSizerFlags().Expand());

    wxBoxSizer* loginSizer = new wxBoxSizer(wxHORIZONTAL);
    loginSizer->Add(loginInnerSizer, wxSizerFlags().Expand().Border(wxALL, 10));

    SetSizer(loginSizer);
    Fit();
}

void LoginDialog::handleClose(wxCloseEvent& WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}

void LoginDialog::handleBrowseUserPrivateKeyFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog fileDialog{ this, "Select Private Key File" , wxStandardPaths::Get().GetAppDocumentsDir(), wxEmptyString, "*.pem" };
    fileDialog.ShowModal();
    m_userPrivateKeyFilename = fileDialog.GetPath();
    m_userPrivateKeyFilenameTextbox->ChangeValue(fileDialog.GetFilename());
}

void LoginDialog::handleBrowseBankPublicKeyFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog fileDialog{ this, "Select bank Public Key File" , wxStandardPaths::Get().GetAppDocumentsDir(), wxEmptyString, "*.pem" };
    fileDialog.ShowModal();
    m_bankPublicKeyFilename = fileDialog.GetPath();
    m_bankPublicKeyFilenameTextbox->ChangeValue(fileDialog.GetFilename());
}

void LoginDialog::handleLogIn(wxCommandEvent& WXUNUSED(event))
{
    if (TransferDataFromWindow() && Validate())
    {
        if (IsModal())
            EndModal(LOGIN_LogIn);
        else
        {
            SetReturnCode(LOGIN_LogIn);
            this->Show(false);
        }
    }
}

void LoginDialog::handleCancel(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

wxString LoginDialog::getUserAccountNumber() const
{
    return m_userAccountNumber;
}

wxString LoginDialog::getUserPassword() const
{
    return m_userPassword;
}

wxString LoginDialog::getUserPrivateKeyFilename() const
{
    return m_userPrivateKeyFilename;
}

wxString LoginDialog::getBankIPAddress() const
{
    return m_bankIPAddress;
}

wxString LoginDialog::getBankPort() const
{
    return m_bankPort;
}

wxString LoginDialog::getBankPublicKeyFilename() const
{
    return m_bankPublicKeyFilename;
}

LoginDialog::UserAccountNumberValidator::UserAccountNumberValidator(wxString* value) :
    wxTextValidator{ wxFILTER_INCLUDE_CHAR_LIST, value },
    m_errorCaption{ "Please enter your 12 digits account number. Spaces are ignored." },
    m_errorMessage{ "Invalid account number" }
{
    SetCharIncludes("0123456789 ");
}

wxObject* LoginDialog::UserAccountNumberValidator::Clone() const
{
    return new UserAccountNumberValidator(*this);
}

bool LoginDialog::UserAccountNumberValidator::Validate(wxWindow* parent)
{
    m_stringValue->erase(std::remove_if(m_stringValue->begin(), m_stringValue->end(), [](char c) { return c == ' '; }), m_stringValue->end());
    if (m_stringValue->length() == 12)
        return true;
    else
    {
        errorBox(parent, m_errorMessage, m_errorCaption);
        return false;
    }
}

LoginDialog::NonEmptyValidator::NonEmptyValidator(wxString* value,
                                                  wxString const& errorCaption,
                                                  wxString const& errorMessage) :
    wxTextValidator{ wxFILTER_EMPTY, value },
    m_errorCaption{ errorCaption },
    m_errorMessage{ errorMessage }
{
}

wxObject* LoginDialog::NonEmptyValidator::Clone() const
{
    return new NonEmptyValidator(*this);
}

bool LoginDialog::NonEmptyValidator::Validate(wxWindow* parent)
{
    if (!m_stringValue->empty())
        return true;
    else
    {
        errorBox(parent, m_errorMessage, m_errorCaption);
        return false;
    }
}

} // namespace clientapp
} // namespace jm
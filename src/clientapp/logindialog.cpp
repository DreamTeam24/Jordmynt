#include "logindialog.h"
#include "gui.h"
#include "errorbox.h"
#include <wx/statline.h>
#include <wx/stdpaths.h>
#include <algorithm>

namespace jm {
namespace clientapp {

enum
{
    LOGIN_BrowseClientPrivateKeyFile = 1,
    LOGIN_BrowseServerPublicKeyFile = 2,
    LOGIN_LogIn = wxID_OK,
    LOGIN_Cancel = wxID_CANCEL
};

wxBEGIN_EVENT_TABLE(LoginDialog, wxDialog)
    EVT_CLOSE(LoginDialog::handleClose)
    EVT_BUTTON(LOGIN_BrowseClientPrivateKeyFile, LoginDialog::handleBrowseClientPrivateKeyFile)
    EVT_BUTTON(LOGIN_BrowseServerPublicKeyFile, LoginDialog::handleBrowseServerPublicKeyFile)
    EVT_BUTTON(LOGIN_LogIn, LoginDialog::handleLogIn)
    EVT_BUTTON(LOGIN_Cancel, LoginDialog::handleCancel)
wxEND_EVENT_TABLE()

LoginDialog::LoginDialog(GUI* gui) :
    wxDialog{ gui, wxID_ANY, "Log in", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE },
    m_gui{ gui },
    m_clientAccountNumber{},
    m_clientAccountNumberLabel{ new wxStaticText(this, wxID_ANY, "My account number") },
    m_clientAccountNumberTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER,
                                  ClientAccountNumberValidator{ &m_clientAccountNumber }) },
    m_clientPassword{},
    m_clientPasswordLabel{ new wxStaticText(this, wxID_ANY, "My password") },
    m_clientPasswordTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD) },
    m_clientPrivateKeyFilename{},
    m_clientPrivateKeyFilenameLabel{ new wxStaticText(this, wxID_ANY, "My private key file") },
    m_clientPrivateKeyFilenameTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY,
                                       NonEmptyValidator{ &m_clientPrivateKeyFilename, "Invalid private key file", "Please select your private key file."})},
    m_clientPrivateKeyFilenameButton{ new wxButton(this, LOGIN_BrowseClientPrivateKeyFile, "Browse...") },
    m_serverIPAddress{},
    m_serverIPAddressLabel{ new wxStaticText(this, wxID_ANY, "Bank IP address or domain name") },
    m_serverIPAddressTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER,
                              NonEmptyValidator{ &m_serverIPAddress, "Invalid IP address or domain name", "Please enter the bank's IP address or domain name." }) },
    m_serverPort{},
    m_serverPortLabel{ new wxStaticText(this, wxID_ANY, "Bank port or service name") },
    m_serverPortTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER,
                         NonEmptyValidator{ &m_serverPort, "Invalid port or service name", "Please enter the bank's port or service name." }) },
    m_serverPublicKeyFilename{},
    m_serverPublicKeyFilenameLabel{ new wxStaticText(this, wxID_ANY, "Bank public key file") },
    m_serverPublicKeyFilenameTextbox{ new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY,
                                      NonEmptyValidator{ &m_serverPublicKeyFilename, "Invalid public key file", "Please select the bank's public key file." })},
    m_serverPublicKeyFilenameButton{ new wxButton(this, LOGIN_BrowseServerPublicKeyFile, "Browse...") },
    m_loginButton{ new wxButton(this, LOGIN_LogIn , "Log in") },
    m_cancelButton{ new wxButton(this, LOGIN_Cancel, "Cancel") }
{
    m_clientAccountNumberTextbox->SetHint("0000 0000 0000");

    wxBoxSizer* clientPrivateKeyFilenameSizer = new wxBoxSizer(wxHORIZONTAL);
    clientPrivateKeyFilenameSizer->Add(m_clientPrivateKeyFilenameTextbox, wxSizerFlags(1));
    clientPrivateKeyFilenameSizer->AddSpacer(10);
    clientPrivateKeyFilenameSizer->Add(m_clientPrivateKeyFilenameButton);

    wxBoxSizer* serverPublicKeyFilenameSizer = new wxBoxSizer(wxHORIZONTAL);
    serverPublicKeyFilenameSizer->Add(m_serverPublicKeyFilenameTextbox, wxSizerFlags(1));
    serverPublicKeyFilenameSizer->AddSpacer(10);
    serverPublicKeyFilenameSizer->Add(m_serverPublicKeyFilenameButton);

    wxBoxSizer* buttonInnerSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonInnerSizer->Add(m_loginButton);
    buttonInnerSizer->AddSpacer(10);
    buttonInnerSizer->Add(m_cancelButton);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(buttonInnerSizer, wxSizerFlags().Right());
    
    wxBoxSizer* loginInnerSizer = new wxBoxSizer(wxVERTICAL);
    loginInnerSizer->Add(m_clientAccountNumberLabel);
    loginInnerSizer->Add(m_clientAccountNumberTextbox, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(m_clientPasswordLabel);
    loginInnerSizer->Add(m_clientPasswordTextbox, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(m_clientPrivateKeyFilenameLabel);
    loginInnerSizer->Add(clientPrivateKeyFilenameSizer, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(15);
    loginInnerSizer->Add(new wxStaticLine(this), wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(m_serverIPAddressLabel);
    loginInnerSizer->Add(m_serverIPAddressTextbox, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(m_serverPortLabel);
    loginInnerSizer->Add(m_serverPortTextbox, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(m_serverPublicKeyFilenameLabel);
    loginInnerSizer->Add(serverPublicKeyFilenameSizer, wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(15);
    loginInnerSizer->Add(new wxStaticLine(this), wxSizerFlags().Expand());
    loginInnerSizer->AddSpacer(10);
    loginInnerSizer->Add(buttonSizer, wxSizerFlags().Expand());

    wxBoxSizer* loginSizer = new wxBoxSizer(wxHORIZONTAL);
    loginSizer->Add(loginInnerSizer, wxSizerFlags().Expand().Border(wxALL, 10));

    SetSizer(loginSizer);
    Fit();
}

GUI* LoginDialog::getGUI() const
{
    return m_gui;
}

std::string LoginDialog::getClientAccountNumber() const
{
    return m_clientAccountNumber.ToStdString();
}

std::string LoginDialog::getClientPassword() const
{
    return m_clientPassword.ToStdString();
}

std::string LoginDialog::getClientPrivateKeyFilename() const
{
    return m_clientPrivateKeyFilename.ToStdString();
}

std::string LoginDialog::getServerIPAddress() const
{
    return m_serverIPAddress.ToStdString();
}

std::string LoginDialog::getServerPort() const
{
    return m_serverPort.ToStdString();
}

std::string LoginDialog::getServerPublicKeyFilename() const
{
    return m_serverPublicKeyFilename.ToStdString();
}

void LoginDialog::handleClose(wxCloseEvent& WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}

void LoginDialog::handleBrowseClientPrivateKeyFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog fileDialog{ this, "Select Your Private Key File" , wxStandardPaths::Get().GetAppDocumentsDir(), wxEmptyString, "*.pem" };
    fileDialog.ShowModal();
    m_clientPrivateKeyFilename = fileDialog.GetPath();
    m_clientPrivateKeyFilenameTextbox->ChangeValue(fileDialog.GetFilename());
}

void LoginDialog::handleBrowseServerPublicKeyFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog fileDialog{ this, "Select Bank Public Key File" , wxStandardPaths::Get().GetAppDocumentsDir(), wxEmptyString, "*.pem" };
    fileDialog.ShowModal();
    m_serverPublicKeyFilename = fileDialog.GetPath();
    m_serverPublicKeyFilenameTextbox->ChangeValue(fileDialog.GetFilename());
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

LoginDialog::ClientAccountNumberValidator::ClientAccountNumberValidator(wxString* value) :
    wxTextValidator{ wxFILTER_INCLUDE_CHAR_LIST, value },
    m_errorCaption{ "Please enter your 12 digits account number. Spaces are ignored." },
    m_errorMessage{ "Invalid account number" }
{
    SetCharIncludes("0123456789 ");
}

wxObject* LoginDialog::ClientAccountNumberValidator::Clone() const
{
    return new ClientAccountNumberValidator(*this);
}

bool LoginDialog::ClientAccountNumberValidator::Validate(wxWindow* parent)
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
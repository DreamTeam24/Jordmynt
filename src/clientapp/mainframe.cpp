#include "mainframe.h"
#include "logindialog.h"
#include "app.h"
//#include "id.h"
//#include "crypto/bytes.h"
//#include "crypto/conversions.h"
//#include "logger/logger.h"
//#include <wx/statline.h>
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "../sample.xpm"
#endif
//#include <thread>

namespace jm {
namespace clientapp {

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_CLOSE(MainFrame::handleClose)
    EVT_MENU(wxID_EXIT, MainFrame::handleExit)
    //EVT_MENU(ID_About, MainFrame::handleAbout)
    //EVT_MENU(ID_Connect, MainFrame::handleConnect)
    //EVT_MENU(ID_Disconnect, MainFrame::handleDisconnect)
    //EVT_MENU(ID_Send, MainFrame::handleSend)
    //EVT_THREAD(ID_Start, MainFrame::handleStart)
    //EVT_THREAD(ID_Receive, MainFrame::handleReceive)
    //EVT_THREAD(ID_Stop, MainFrame::handleStop)
    //EVT_BUTTON(ID_BrowsePrivateKeyClient, MainFrame::popUp)
    //EVT_BUTTON(ID_Setup, MainFrame::handleSetup)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(wxString const& title) :
    wxFrame(nullptr, wxID_ANY, title),
    m_ioContext{},
    m_app{ std::make_shared<App>(this) },
    m_client{ m_ioContext, m_app }
{
    SetIcon(wxICON(sample));

    wxMenu* fileMenu = new wxMenu;
    //fileMenu->Append(ID_Connect, "C&onnect\tAlt-C", "Connect to the server");
    //fileMenu->Append(ID_Send, "S&end\tAlt-S", "Send message");
    //fileMenu->Append(ID_Disconnect, "D&isconnect\tAlt-C", "Disconnect from the server");
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Exit this program");

    //wxMenu* helpMenu = new wxMenu;
    //helpMenu->Append(ID_About, "&About\tF1", "Show about dialog");

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    //menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);

    //CreateStatusBar(2);
    //SetStatusText("Welcome to wxWidgets!");

    LoginDialog loginDialog{ this };
    if (loginDialog.ShowModal() == wxID_OK)
    {
        /*m_ioContext.restart();
        m_client.connect("127.0.0.1", "7");
        std::thread networkCommunication{
            [this]()
            {
                m_ioContext.run();
            }
        };
        networkCommunication.detach();*/
        
        //wxMessageBox(loginDialog.getUserAccountNumber(), "gelukt!!!");
    }
    else
    {
        wxMessageBox(":-(", "mislukt");
    }

    /*wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxStaticText* accountNumberLabel = new wxStaticText(panel, wxID_ANY, "Account number");
    wxTextCtrl* accountNumberTextbox = new wxTextCtrl(panel, wxID_ANY);
    std::string accountNumberHint{ "0000 0000 0000" };
    accountNumberTextbox->SetHint(accountNumberHint);
    accountNumberTextbox->SetInitialSize(accountNumberTextbox->GetSizeFromTextSize(accountNumberTextbox->GetTextExtent(accountNumberHint).x));
    accountNumberTextbox->SetMaxLength(accountNumberHint.size());
    wxTextValidator accountNumberValidator{ wxFILTER_INCLUDE_CHAR_LIST };
    accountNumberValidator.SetCharIncludes("0123456789 ");
    accountNumberTextbox->SetValidator(accountNumberValidator);

    wxStaticText* passwordLabel = new wxStaticText(panel, wxID_ANY, "Password");
    wxTextCtrl* passwordTextbox = new wxTextCtrl(panel, wxID_ANY);

    wxStaticText* privateKeyLabel = new wxStaticText(panel, wxID_ANY, "Private key file");
    wxTextCtrl* privateKeyTextbox = new wxTextCtrl(panel, wxID_ANY);
    std::string privateKeyHint{ "path/to/your_private_key.pem" };
    privateKeyTextbox->SetHint(privateKeyHint);
    privateKeyTextbox->SetInitialSize(privateKeyTextbox->GetSizeFromTextSize(privateKeyTextbox->GetTextExtent(privateKeyHint).x));
    wxButton* privateKeyButton = new wxButton(panel, wxID_ANY, "Browse...");
    wxBoxSizer* privateKeySizer = new wxBoxSizer(wxHORIZONTAL);
    privateKeySizer->Add(privateKeyTextbox, wxSizerFlags(1));
    privateKeySizer->AddSpacer(10);
    privateKeySizer->Add(privateKeyButton);

    wxStaticText* bankLabel = new wxStaticText(panel, wxID_ANY, "Bank");
    wxTextCtrl* bankTextbox = new wxTextCtrl(panel, wxID_ANY);
    wxButton* bankButton = new wxButton(panel, ID_Setup, "Set up...");
    wxBoxSizer* bankSizer = new wxBoxSizer(wxHORIZONTAL);
    bankSizer->Add(bankTextbox, wxSizerFlags(1));
    bankSizer->AddSpacer(10);
    bankSizer->Add(bankButton);

    wxStaticLine* line = new wxStaticLine(panel);

    wxButton* loginButton = new wxButton(panel, wxID_ANY, "Log in");

    wxBoxSizer* innerSizer = new wxBoxSizer(wxVERTICAL);
    innerSizer->Add(accountNumberLabel);
    innerSizer->Add(accountNumberTextbox);
    innerSizer->AddSpacer(10);
    innerSizer->Add(passwordLabel);
    innerSizer->Add(passwordTextbox, wxSizerFlags().Expand());
    innerSizer->AddSpacer(10);
    innerSizer->Add(privateKeyLabel);
    innerSizer->Add(privateKeySizer, wxSizerFlags().Expand());
    innerSizer->AddSpacer(10);
    innerSizer->Add(bankLabel);
    innerSizer->Add(bankSizer, wxSizerFlags().Expand());
    innerSizer->AddSpacer(10);
    innerSizer->Add(line, wxSizerFlags().Expand());
    innerSizer->AddSpacer(10);
    innerSizer->Add(loginButton, wxSizerFlags().Center());

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(innerSizer, wxSizerFlags().Expand().Border(wxALL, 5));

    panel->SetSizer(mainSizer);
    panel->Fit();
    Fit();
    SetMinSize(GetSize());
    SetMaxSize(wxSize(5 * GetSize().x, GetSize().y));*/

    /*wxStaticBox* yourBox = new wxStaticBox(panel, wxID_ANY, "You");

    wxStaticText* yourAccountNumberLabel = new wxStaticText(yourBox, wxID_ANY, "Account number");
    wxTextCtrl* yourAccountNumberTextbox = new wxTextCtrl(yourBox, wxID_ANY);
    std::string yourAccountNumberHint{ "0000 0000 0000" };
    yourAccountNumberTextbox->SetHint(yourAccountNumberHint);
    yourAccountNumberTextbox->SetInitialSize(yourAccountNumberTextbox->GetSizeFromTextSize(yourAccountNumberTextbox->GetTextExtent(yourAccountNumberHint).x));
    yourAccountNumberTextbox->SetMaxLength(yourAccountNumberHint.size());
    wxTextValidator yourAccountNumberValidator{ wxFILTER_INCLUDE_CHAR_LIST };
    yourAccountNumberValidator.SetCharIncludes("0123456789 ");
    yourAccountNumberTextbox->SetValidator(yourAccountNumberValidator);

    wxStaticText* yourPasswordLabel = new wxStaticText(yourBox, wxID_ANY, "Password");
    wxTextCtrl* yourPasswordTextbox = new wxTextCtrl(yourBox, wxID_ANY);

    wxStaticText* yourPrivateKeyLabel = new wxStaticText(yourBox, wxID_ANY, "Private key file");
    wxTextCtrl* yourPrivateKeyTextbox = new wxTextCtrl(yourBox, wxID_ANY);
    std::string yourPrivateKeyHint{ "path/to/your_private_key.pem" };
    yourPrivateKeyTextbox->SetHint(yourPrivateKeyHint);
    yourPrivateKeyTextbox->SetInitialSize(yourPrivateKeyTextbox->GetSizeFromTextSize(yourPrivateKeyTextbox->GetTextExtent(yourPrivateKeyHint).x));
    wxButton* yourPrivateKeyButton = new wxButton(yourBox, wxID_ANY, "Browse...");
    wxBoxSizer* yourPrivateKeySizer = new wxBoxSizer(wxHORIZONTAL);
    yourPrivateKeySizer->Add(yourPrivateKeyTextbox , wxSizerFlags(1));
    yourPrivateKeySizer->AddSpacer(10);
    yourPrivateKeySizer->Add(yourPrivateKeyButton);

    wxBoxSizer* yourInnerSizer = new wxBoxSizer(wxVERTICAL);
    yourInnerSizer->Add(yourAccountNumberLabel);
    yourInnerSizer->Add(yourAccountNumberTextbox);
    yourInnerSizer->AddSpacer(10);
    yourInnerSizer->Add(yourPasswordLabel);
    yourInnerSizer->Add(yourPasswordTextbox, wxSizerFlags().Expand());
    yourInnerSizer->AddSpacer(10);
    yourInnerSizer->Add(yourPrivateKeyLabel);
    yourInnerSizer->Add(yourPrivateKeySizer, wxSizerFlags().Expand());

    wxStaticBoxSizer* yourSizer = new wxStaticBoxSizer(yourBox, wxVERTICAL);
    yourSizer->Add(yourInnerSizer, wxSizerFlags().Expand().Border(wxALL, 5));

    wxStaticBox* bankBox = new wxStaticBox(panel, wxID_ANY, "Bank");

    wxStaticText* bankIPAddressLabel = new wxStaticText(bankBox, wxID_ANY, "IP address or domain name");
    wxTextCtrl* bankIPAddressTextbox = new wxTextCtrl(bankBox, wxID_ANY);

    wxStaticText* bankPortLabel = new wxStaticText(bankBox, wxID_ANY, "Port or service name");
    wxTextCtrl* bankPortTextbox = new wxTextCtrl(bankBox, wxID_ANY);
    
    wxStaticText* bankPublicKeyLabel = new wxStaticText(bankBox, wxID_ANY, "Public key file");
    wxTextCtrl* bankPublicKeyTextbox = new wxTextCtrl(bankBox, wxID_ANY);
    std::string bankPublicKeyHint{ "path/to/bank_public_key.pem" };
    bankPublicKeyTextbox->SetHint(bankPublicKeyHint);
    bankPublicKeyTextbox->SetInitialSize(bankPublicKeyTextbox->GetSizeFromTextSize(bankPublicKeyTextbox->GetTextExtent(bankPublicKeyHint).x));
    wxButton* bankPublicKeyButton = new wxButton(bankBox, wxID_ANY, "Browse...");
    wxBoxSizer* bankPublicKeySizer = new wxBoxSizer(wxHORIZONTAL); 
    bankPublicKeySizer->Add(bankPublicKeyTextbox, wxSizerFlags(1));
    bankPublicKeySizer->AddSpacer(10);
    bankPublicKeySizer->Add(bankPublicKeyButton);

    wxBoxSizer* bankInnerSizer = new wxBoxSizer(wxVERTICAL);
    bankInnerSizer->Add(bankIPAddressLabel);
    bankInnerSizer->Add(bankIPAddressTextbox, wxSizerFlags().Expand());
    bankInnerSizer->AddSpacer(10); 
    bankInnerSizer->Add(bankPortLabel);
    bankInnerSizer->Add(bankPortTextbox, wxSizerFlags().Expand());
    bankInnerSizer->AddSpacer(10);
    bankInnerSizer->Add(bankPublicKeyLabel);
    bankInnerSizer->Add(bankPublicKeySizer, wxSizerFlags().Expand());

    wxStaticBoxSizer* bankSizer = new wxStaticBoxSizer(bankBox, wxVERTICAL);
    bankSizer->Add(bankInnerSizer, wxSizerFlags().Expand().Border(wxALL, 5));

    wxStaticLine* line = new wxStaticLine(panel);

    wxButton* connectButton = new wxButton(panel, wxID_ANY, "Connect");

    wxBoxSizer* mainInnerSizer = new wxBoxSizer(wxVERTICAL);
    mainInnerSizer->Add(yourSizer, wxSizerFlags().Expand());
    mainInnerSizer->AddSpacer(10);
    mainInnerSizer->Add(bankSizer, wxSizerFlags().Expand());
    mainInnerSizer->AddSpacer(10);
    mainInnerSizer->Add(line, wxSizerFlags().Expand());
    mainInnerSizer->AddSpacer(10);
    mainInnerSizer->Add(connectButton, wxSizerFlags().Right());

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(mainInnerSizer, wxSizerFlags().Expand().Border(wxALL, 5));

    panel->SetSizer(mainSizer);
    panel->Fit();
    Fit();
    SetMinSize(GetSize());
    SetMaxSize(wxSize(2*GetSize().x, GetSize().y));*/

    //int w4{ mainSizer->GetMinSize().x };
    //int h4{ mainSizer->GetMinSize().y };

    //panel->SetMinSize(mainSizer->GetMinSize());

    //int w5{ panel->GetMinSize().x };
    //int h5{ panel->GetMinSize().y };

    //SetMinSize(panel->GetMinSize());
    ////SetMinSize(wxSize(w5,h5));

    //int w6{ GetMinSize().x };
    //int h6{ GetMinSize().y };

    //panel->GetSizer()->Fit(this);

    //int labelWidth{std::max(accountNumberFromText->GetSize().x, privateKeyText->GetSize().x)};
    //accountNumberFromText->SetMinSize(wxSize(labelWidth, accountNumberFromText->GetMinHeight()));
    //privateKeyText->SetMinSize(wxSize(labelWidth, privateKeyText->GetMinHeight()));

    //wxTextCtrl* accountNumberToBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);

    //wxTextCtrl* privateKeyClientBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    //wxTextCtrl* publicKeyServerBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);

    //wxTextCtrl* ipAddressServerBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    //wxTextCtrl* portServerBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    //
    //wxButton* browsePrivateKeyClientButton = new wxButton(panel, wxID_ANY, "Browse");
    //wxButton* browsePublicKeyServerButton = new wxButton(panel, wxID_ANY, "Browse");

    //wxTextCtrl* amountBox = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
    //
    //wxStaticText* balanceText = new wxStaticText(panel, wxID_ANY, wxEmptyString);
    //wxStaticText* receivableText = new wxStaticText(panel, wxID_ANY, wxEmptyString);
    //wxStaticText* spendableText = new wxStaticText(panel, wxID_ANY, wxEmptyString);
}

void MainFrame::handleClose(wxCloseEvent& event)
{
    Destroy();
}

void MainFrame::handleExit(wxCommandEvent& event)
{
    Close();
}

//void MainFrame::handleAbout(wxCommandEvent& event)
//{
//    wxMessageBox(wxString::Format
//        (
//            "Welcome to %s!\n"
//            "\n"
//            "This is the minimal wxWidgets sample\n"
//            "running under %s.",
//            wxVERSION_STRING,
//            wxGetOsDescription()
//        ),
//        "About wxWidgets minimal sample",
//        wxOK | wxICON_INFORMATION,
//        this);
//}
//
//void MainFrame::handleSetup(wxCommandEvent& event)
//{
//    BankFrame* bankFrame = new BankFrame("test");
//    bankFrame->Show(true);
//}
//
//void MainFrame::handleConnect(wxCommandEvent& event)
//{
//    m_ioContext.restart();
//    m_client.connect("127.0.0.1", "7");
//    std::thread networkCommunication{
//        [this]()
//        {
//            m_ioContext.run();
//        }
//    };
//    networkCommunication.detach();
//    this->GetMenuBar()->Enable(ID_Connect, false);
//    this->GetMenuBar()->Enable(ID_Disconnect, true);
//}
//
//void MainFrame::handleDisconnect(wxCommandEvent& event)
//{
//    m_client.disconnect();
//    this->GetMenuBar()->Enable(ID_Connect, true);
//    this->GetMenuBar()->Enable(ID_Disconnect, false);
//}
//
//void MainFrame::handleSend(wxCommandEvent& event)
//{
//    m_app->setMessageForServer(crypto::convertTextStringToBytes("Hello"));
//    m_client.send();
//}
//
//void MainFrame::handleStart(wxThreadEvent& event)
//{
//    wxMessageBox("Application started", "Started", wxOK, this);
//}
//
//void MainFrame::handleReceive(wxThreadEvent& event)
//{
//    std::string response{ crypto::convertBytesToTextString(m_app->getResponseFromServer()) };
//    wxMessageBox(wxString(response), "Response", wxOK, this);
//}
//
//void MainFrame::handleStop(wxThreadEvent& event)
//{
//    wxMessageBox("Application stopped", "Stopped", wxOK, this);
//}
//
//void MainFrame::popUp(wxCommandEvent& event)
//{
//    wxMessageBox("Jaaaaa!!!!!", "Joepie", wxOK, this);
//}

} // namespace clientapp
} // namespace jm
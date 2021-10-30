/*#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class DIALOG_WRAPTEXT : public wxDialog
{
public:

    DIALOG_WRAPTEXT(wxWindow* parent,
        const wxString& aTitle, const wxSize aSize);

private:

    void onResize(wxSizeEvent& evt);

    wxBoxSizer* m_itemBoxSizer;
    wxStaticText* m_text;
    wxStaticText* m_text2;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(DIALOG_WRAPTEXT, wxDialog)
    EVT_SIZE(DIALOG_WRAPTEXT::onResize)
wxEND_EVENT_TABLE()

DIALOG_WRAPTEXT::DIALOG_WRAPTEXT(
    wxWindow* parent, const wxString& aTitle, const wxSize aSize) :
    wxDialog(parent, wxID_ANY, aTitle,
        wxPoint(-1, -1), aSize,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    wxString msg("Lots and lots of text to wrap hopefully. "
        "Lots and lots of text to wrap hopefully. "
        "Lots and lots of text to wrap hopefully. "
        "Lots and lots of text to wrap hopefully. "
        "Lots and lots of text to wrap hopefully. "
        "Lots and lots of text to wrap hopefully. "
    );

    m_text = new wxStaticText(this, wxID_ANY, msg);
    
    wxString msg2("Lots and lots of text to wrap hopefully. "
        "More lots of text to wrap hopefully. "
        "Lots and lots of text to wrap hopefully. "
        "Lots and lots of text to wrap hopefully. "
        "Lots and lots of text to wrap hopefully. "
        "Lots and lots of text to wrap hopefully. "
    );
    
    m_text2 = new wxStaticText(this, wxID_ANY, msg);
    
    m_itemBoxSizer = new wxBoxSizer(wxVERTICAL);
    

    
    // wxEXPAND makes no difference
    m_itemBoxSizer->Add(m_text, 1, wxALIGN_TOP | wxALL | wxEXPAND, 5);
    m_itemBoxSizer->Add(m_text2, 1, wxALIGN_TOP | wxALL | wxEXPAND, 5);

    SetSizer(m_itemBoxSizer);
}

void DIALOG_WRAPTEXT::onResize(wxSizeEvent& event)
{
    // layout everything in the dialog
    Layout();
    event.Skip();
}


class MyApp : public wxApp
{
public:

    bool OnInit() override
    {
        auto d = new DIALOG_WRAPTEXT(NULL, "Dialog title", wxSize(200, 200));

        d->ShowModal();
        d->Destroy();

        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
*/

#include "mainframe.h"
#include "main.h"

namespace jm {
namespace clientapp {

bool Main::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    MainFrame* mainFrame = new MainFrame("Client app");
    mainFrame->Show(true);

    return true;
}

wxIMPLEMENT_APP(Main);

} // namespace clientapp
} // namespace jm




// generate key pair
// load key pair


/*#include <jm/client/client.h>
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char** argv)
{    
    try
    {
        // Create client
        boost::asio::io_context ioContext{};
        jm::client::Client client{ ioContext };// "client_private.pem")
        client.connect("127.0.0.1", "7");// , "server_public.pem");

        // Run asynchronous calls
        ioContext.run();
    }
    catch (std::exception exception)
    {
        std::cerr << exception.what() << std::endl;
    }

    system("pause");

    return 0;
}*/

//#include <openssl/evp.h>
//#include <openssl/pem.h>
//
//int main(int argc, char** argv)
//{
//    // Generate private and public key
//    EVP_PKEY* pkey = NULL;
//    EVP_PKEY_CTX* pctx = EVP_PKEY_CTX_new_id(NID_KeyPair, NULL);
//    EVP_PKEY_keygen_init(pctx);
//    EVP_PKEY_keygen(pctx, &pkey);
//
//    // Write private and public key to file
//    BIO* out;
//    out = BIO_new_file("client_private.pem", "w+");
//    PEM_write_bio_PrivateKey(out, pkey, nullptr, nullptr, 0, nullptr, nullptr);
//    BIO_flush(out);
//    out = BIO_new_file("client_public.pem", "w+");
//    PEM_write_bio_PUBKEY(out, pkey);
//    BIO_flush(out);
//
//    // Free memory
//    EVP_PKEY_CTX_free(pctx);
//    EVP_PKEY_free(pkey);
//    BIO_free(out);
//
//    return 0;
//}
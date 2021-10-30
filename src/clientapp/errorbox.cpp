#include "errorbox.h"

namespace jm {
namespace clientapp {

void errorBox(wxWindow*       parent,
              wxString const& errorCaption,
              wxString const& errorMessage)
{
    wxMessageDialog error{ parent, errorMessage, errorCaption, wxOK | wxICON_ERROR };
    error.SetOKLabel("Close");
    error.ShowModal();
}

} // namespace clientapp
} // namespace jm
#ifndef JM_CLIENTAPP_ERRORBOX_H
#define JM_CLIENTAPP_ERRORBOX_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

namespace jm {
namespace clientapp {

// Alternative for wxMessageBox to show an error with a 'Close' button instead of an 'OK' button.
void errorBox(wxWindow*       parent,
              wxString const& errorCaption,
              wxString const& errorMessage);

} // namespace clientapp
} // namespace jm

#endif // JM_CLIENTAPP_ERRORBOX_H
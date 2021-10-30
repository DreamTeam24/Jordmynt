#ifndef JM_CLIENTAPP_MAIN_H
#define JM_CLIENTAPP_MAIN_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

namespace jm {
namespace clientapp {

class Main : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};

DECLARE_APP(Main);

} // namespace clientapp
} // namespace jm

#endif // JM_CLIENTAPP_MAIN_H
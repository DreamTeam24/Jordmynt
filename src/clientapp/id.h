#ifndef JM_CLIENTAPP_ID_H
#define JM_CLIENTAPP_ID_H

namespace jm {
namespace clientapp {

enum
{
    ID_Exit = wxID_EXIT,
    ID_About = wxID_ABOUT,
    ID_Connect = 1,
    ID_Disconnect = 2,
    ID_Start = 3,
    ID_Stop = 4,
    ID_Send = 5,
    ID_Receive = 6,
    ID_BrowsePrivateKeyClient = 7,
    LOGIN_AccountNumber = 8,
    LOGIN_Password = 9,
    LOGIN_PrivateKey = 10
};

} // namespace clientapp
} // namespace jm

#endif // JM_CLIENTAPP_ID_H
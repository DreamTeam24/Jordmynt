#ifndef JM_CLIENT_APP_H
#define JM_CLIENT_APP_H

#include "crypto/bytes.h"
#include <memory>

namespace jm {
namespace client {

class App : public std::enable_shared_from_this<App>
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual crypto::Bytes getMessageForServer() = 0;
    virtual void setResponseFromServer(crypto::Bytes response) = 0;
};

} // namespace client
} // namespace jm

#endif // JM_CLIENT_APP_H
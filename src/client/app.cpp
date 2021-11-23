#include "app.h"

#include "logger/logger.h"

namespace jm {
namespace client {

App::App() :
    m_client{ std::make_unique<Client>(this) }
{
}

} // namespace client
} // namespace jm
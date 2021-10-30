#ifndef JM_LOGGER_LOGGER_H
#define JM_LOGGER_LOGGER_H

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <string>

namespace jm {
namespace logger {

void DEBUG(std::string const& message);
void INFO(std::string const& message);
void WARNING(std::string const& message);
void THROW(std::string const& message);

} // namespace log
} // namespace jm

#endif // JM_LOGGER_LOGGER_H
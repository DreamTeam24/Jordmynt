#include "logger.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <ostream>
#include <stdexcept>

namespace jm {
namespace logger {

enum class SeverityLevel
{
    debug,
    info,
    warning,
    error
};

std::ostream& operator<< (std::ostream& stream, SeverityLevel level)
{
    switch (level)
    {
    case SeverityLevel::debug:
        stream << "debug";
        break;
    case SeverityLevel::info:
        stream << "info";
        break;
    case SeverityLevel::warning:
        stream << "warning";
        break;
    case SeverityLevel::error:
        stream << "error";
        break;
    default:
        stream << static_cast<int>(level);
    }
    return stream;
}

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", SeverityLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(Logger, boost::log::sources::severity_logger<SeverityLevel>)
{
    boost::log::sources::severity_logger<SeverityLevel> globalLogger;
    
    // Setup formatter for all sinks
    boost::log::formatter formatter = boost::log::expressions::stream
        << "[" << timestamp << "] " << severity << ":\t" << boost::log::expressions::smessage;

    // Initialize sinks
    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > file_sink;
    boost::shared_ptr< file_sink > file = boost::log::add_file_log("jm_%N.log");
    file->set_formatter(formatter);
    file->locked_backend()->set_rotation_size(10 * 1024 * 1024);

    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > console_sink;
    boost::shared_ptr< console_sink > console = boost::log::add_console_log();
    console->set_formatter(formatter);
    console->set_filter(severity >= SeverityLevel::info);
    console->locked_backend()->auto_flush(true);

    // Add attributes
    boost::log::add_common_attributes();

    return globalLogger;
}

void DEBUG(std::string const& message)
{
    BOOST_LOG_SEV(logger::Logger::get(), logger::SeverityLevel::debug) << message;
}

void INFO(std::string const& message)
{
    BOOST_LOG_SEV(logger::Logger::get(), logger::SeverityLevel::info) << message;
}

void WARNING(std::string const& message)
{
    BOOST_LOG_SEV(logger::Logger::get(), logger::SeverityLevel::warning) << message;
}

void THROW(std::string const& message)
{
    BOOST_LOG_SEV(logger::Logger::get(), logger::SeverityLevel::error) << message;
    throw std::runtime_error(message);
}

} // namespace logger
} // namespace jm
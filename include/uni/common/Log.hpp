////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/Log.hpp
/// @brief Logging enums, structs and interface.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 02.2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <mutex>

namespace uni
{
namespace common
{
class Log
{
public:
    enum class Level : uint16_t
    {
        FATAL = 0U,
        ERROR,
        WARNING,
        INFO,
        DEBUG,
        TRACE,
        COUNT  //< Maximum value, used for range check
    };

    template < typename... Args >
    void
    log_msg( Level level, const char* func, const Args&... args )
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        m_ostream << "Level: " << static_cast< std::underlying_type< Level >::type >( level ) << ", " << func << ", ";
        append( m_ostream, args... );
    }

private:
    template < typename T, typename... Args >
    inline void
    append( std::ostream& ostream, const T& value, const Args&... args )
    {
        ostream << value;
        append( ostream, args... );
    }

    template < typename T >
    inline void
    append( std::ostream& ostream, const T& value )
    {
        ostream << value;
    }

private:
    std::mutex m_mutex{};
    std::ostream& m_ostream{ std::cout };
};

Log&
logger( )
{
    static Log log;
    return log;
}

}  // namespace common
}  // namespace uni

#define LOG_FATAL_MSG( ... )   uni::common::logger( ).log_msg( uni::common::Log::Level::FATAL, __func__, __VA_ARGS__ )
#define LOG_ERROR_MSG( ... )   uni::common::logger( ).log_msg( uni::common::Log::Level::ERROR, __func__, __VA_ARGS__ )
#define LOG_WARNING_MSG( ... ) uni::common::logger( ).log_msg( uni::common::Log::Level::WARNING, __func__, __VA_ARGS__ )
#define LOG_INFO_MSG( ... )    uni::common::logger( ).log_msg( uni::common::Log::Level::INFO, __func__, __VA_ARGS__ )
#define LOG_DEBUG_MSG( ... )   uni::common::logger( ).log_msg( uni::common::Log::Level::DEBUG, __func__, __VA_ARGS__ )
#define LOG_TRACE_MSG( ... )   uni::common::logger( ).log_msg( uni::common::Log::Level::TRACE, __func__, __VA_ARGS__ )

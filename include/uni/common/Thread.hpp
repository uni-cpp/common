////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/Thread.hpp
/// @brief Declaration thread class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2020-2021
/// @note Thanks to the POCO library
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uni/common/Constants.hpp"
#include "uni/common/Defines.hpp"
#include "uni/common/ErrorCode.hpp"
#include "uni/common/Log.hpp"
#include "uni/common/Runnable.hpp"

#include <chrono>
#include <condition_variable>
#include <memory>
#include <thread>

namespace uni
{
namespace common
{
class UNI_API Thread : public Runnable
{
public:
    enum class Repeat
    {
        ONCE,
        LOOP,
    };

    struct Settings
    {
        std::string name{};
        Repeat repeat_type{ Repeat::ONCE };
        uint64_t timeout_ms{ DEFAULT_TIMEOUT_MS };

        LOG_CLASS( Settings, LOG_IT( name ), LOG_IT( repeat_type ), LOG_IT( timeout_ms ) );
    };

public:
    Thread( const Settings& settings );

    ~Thread( ) override;

public:
    ErrorCode start( );
    bool is_running( ) const;
    bool is_on_thread( ) const;
    ErrorCode stop( );

    std::string get_name( ) const;

    // Should be re-implemented
protected:
    void run( ) override;

    // Could be re-implemented
protected:
    virtual void on_start( ){};
    virtual void on_stop( ){};

private:
    void prepare_and_run( );
    void join( );

private:
    const Settings m_settings{};

    std::mutex m_mutex{};
    std::condition_variable m_cv{};
    bool m_is_closing{ false };

    std::unique_ptr< std::thread > m_runnable{ nullptr };

    LOG_CLASS( Thread, LOG_IT( m_settings ), LOG_IT( m_is_closing ) );
};

LOG_ENUM( Thread::Repeat, LOG_E( Thread::Repeat::ONCE ), LOG_E( Thread::Repeat::LOOP ) );

void set_current_thread_name( const std::string& name );

}  // namespace common
}  // namespace uni

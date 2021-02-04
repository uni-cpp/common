////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/Thread.hpp
/// @brief Declaration thread class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 08.2020
/// @note Thanks to the POCO library
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uni/common/Constants.hpp"
#include "uni/common/Defines.hpp"
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

public:
    Thread( ) = delete;
    Thread( std::string name, Repeat repeat_type = Repeat::ONCE, uint64_t timeout_ms = DEFAULT_TIMEOUT );

    ~Thread( ) override;

public:
    bool start( );
    bool is_running( ) const;
    bool is_on_thread( ) const;
    bool stop( );

    std::string get_name( ) const;
    static void set_current_thread_name( const std::string& name );

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
    std::string m_name{};
    Repeat m_repeat_type{ Repeat::ONCE };
    uint64_t m_timeout_ms{ 0U };

    std::mutex m_mutex{};
    std::condition_variable m_cv{};
    bool m_is_closing{ false };

    std::unique_ptr< std::thread > m_runnable{ nullptr };
};

}  // namespace common
}  // namespace uni

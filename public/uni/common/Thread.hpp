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
enum class RepeatType
{
    ONCE,
    LOOP,
};

class UNI_API Thread : public Runnable
{
public:
    Thread( ) = delete;
    Thread( std::string name, RepeatType repeat_type = RepeatType::ONCE, uint64_t timeout_ms = DEFAULT_TIMEOUT );

    ~Thread( ) override;

public:
    bool start( );
    bool is_running( ) const;
    bool is_on_thread( ) const;
    bool stop( );

    std::string get_name( ) const;
    static void set_current_thread_name( const std::string& name );


protected:
    virtual void on_start( ){ };
    virtual void on_stop( ){ };


private:
    void prepare_and_run( );
    void join( );

private:
    std::string m_name{ };
    RepeatType m_repeat_type{ RepeatType::ONCE };
    uint64_t m_timeout_ms{ 0U };

    std::mutex m_mutex{ };
    std::condition_variable m_cv{ };
    bool m_is_closing{ false };

    std::unique_ptr< std::thread > m_runnable{ nullptr };
};

}  // namespace common
}  // namespace uni

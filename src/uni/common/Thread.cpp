////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/Thread.cpp
/// @brief Implementation thread class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 23.08.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <uni/common/Log.hpp>
#include <uni/common/Thread.hpp>

#if defined( __WIN64__ )
#include <processthreadsapi.h>
#include <windows.h>
#elif defined( __linux__ )
#include <pthread.h>
#elif defined( __APPLE__ )
#include <pthread.h>
#endif

namespace uni
{
namespace common
{
Thread::~Thread( )
{
    LOG_TRACE_MSG( "" );

    stop( );
}

Thread::Thread( std::string name, Repeat repeat_type, uint64_t timeout_ms )
    : m_name{ std::move( name ) }
    , m_repeat_type{ repeat_type }
    , m_timeout_ms{ timeout_ms }
{
    LOG_TRACE_MSG( "" );
}

ErrorCode
Thread::start( )
{
    LOG_TRACE_MSG( "" );

    if( m_runnable )
    {
        LOG_DEBUG_MSG( "Thread already started" );
        return ErrorCode::INTERNAL;
    }

    on_start( );

    {
        std::lock_guard< std::mutex > lock( m_mutex );
        m_is_closing = false;
    }

    m_runnable = std::make_unique< std::thread >( [this] { prepare_and_run( ); } );
    if( !m_runnable )
    {
        LOG_ERROR_MSG( "Thread was not created." );
        return ErrorCode::INTERNAL;
    }

    return ErrorCode::NONE;
}

bool
Thread::is_running( ) const
{
    LOG_TRACE_MSG( "" );

    return m_runnable && m_runnable->joinable( );
}

bool
Thread::is_on_thread( ) const
{
    LOG_TRACE_MSG( "" );

    return ( m_runnable && ( std::this_thread::get_id( ) == m_runnable->get_id( ) ) );
}

ErrorCode
Thread::stop( )
{
    LOG_TRACE_MSG( "" );

    if( !is_running( ) )
    {
        LOG_DEBUG_MSG( "Thread is not running" );
        return ErrorCode::INTERNAL;
    }

    on_stop( );

    {
        std::lock_guard< std::mutex > lock( m_mutex );
        m_is_closing = true;
        m_cv.notify_one( );
    }

    if( is_on_thread( ) )
    {
        LOG_WARNING_MSG( "Thread was detached. Try to use stop() not from async thread." );
        m_runnable->detach( );
    }
    else
    {
        join( );
    }

    m_runnable.reset( );

    return ErrorCode::NONE;
}

void
Thread::join( )
{
    LOG_TRACE_MSG( "" );

    if( m_runnable && m_runnable->joinable( ) )
    {
        m_runnable->join( );
    }
}

std::string
Thread::get_name( ) const
{
    return m_name;
}

void
set_current_thread_name( const std::string& name )
{
#if defined( __APPLE__ )
    pthread_setname_np( name.c_str( ) );
#elif defined( __linux__ )
    pthread_setname_np( pthread_self( ), name.c_str( ) );
#elif defined( __WIN64__ )
    HRESULT r;
    r = SetThreadDescription( GetCurrentThread( ), std::wstring( name ).c_str( ) );
#endif
}

void
Thread::run( )
{
    LOG_TRACE_MSG( "" );

    // Fix rare pure virtual call
    // Might appears when object of derived class is under destruction before run( ) from async thread is invoked
}

void
Thread::prepare_and_run( )
{
    LOG_TRACE_MSG( "" );

    set_current_thread_name( m_name );

    switch( m_repeat_type )
    {
        case( Repeat::ONCE ):
        {
            run( );
        }
        break;

        case( Repeat::LOOP ):
        {
            auto start_time{ std::chrono::system_clock::now( ) };
            auto end_time{ start_time };
            while( true )
            {
                start_time = std::chrono::system_clock::now( );
                run( );
                end_time = std::chrono::system_clock::now( );

                {
                    std::lock_guard< std::mutex > lock( m_mutex );
                    if( m_is_closing )
                    {
                        break;
                    }
                }

                const auto time_elapsed = end_time - start_time;

                if( time_elapsed < std::chrono::milliseconds( m_timeout_ms ) )
                {
                    const auto time_to_next_run = std::chrono::milliseconds( m_timeout_ms ) - time_elapsed;
                    std::unique_lock< std::mutex > lock( m_mutex );
                    if( m_cv.wait_for( lock, time_to_next_run, [this] { return m_is_closing; } ) )
                    {
                        break;
                    };
                };
            };
        }
        break;
    }
}

}  // namespace common
}  // namespace uni

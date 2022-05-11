////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/ThreadPool.cpp
/// @brief Implementation thread pool class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2020-2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "uni/common/ThreadPool.hpp"
#include "uni/common/Log.hpp"
#include "uni/common/Queue.hpp"

namespace uni
{
namespace common
{
namespace
{
class TaskRunner : public uni::common::Thread
{
public:
    TaskRunner( const Thread::Settings& settings, Queue< DefaultVoidStdFunction >& queue )
        : Thread( settings )
        , m_queue{ queue }
    {
    }

    void
    run( ) override
    {
        DefaultVoidStdFunction task;
        if( OperationStatus::SUCCESS == m_queue.try_pop( task ) )
        {
            task( );
        }
        else
        {
            // Could be sleeped for ~50MS
            std::this_thread::yield( );
        }
    }

private:
    Queue< DefaultVoidStdFunction >& m_queue;
};
}  // namespace


ThreadPool::ThreadPool( const Settings& settings )
{
    LOG_DEBUG_MSG( LOG_IT( settings ) );

    for( uint32_t i = 0; i < settings.thread_count; ++i )
    {
        const std::string thread_name = settings.thread_settings.name + "_" + std::to_string( i );
        Thread::Settings settings{ { thread_name } };

        m_threads.emplace_back( std::make_unique< TaskRunner >( settings, m_queue ) );
    }
}

ThreadPool::~ThreadPool( )
{
    LOG_TRACE_MSG( "" );
    m_is_on_shutdown = true;

    for( auto& thread : m_threads )
    {
        if( thread )
        {
            thread->stop( );
        }
        else
        {
            LOG_ERROR_MSG( "Empty thread" );
        }
    }
}

ErrorCode
ThreadPool::submit( const DefaultVoidStdFunction& task )
{
    REQUIRED( !m_is_on_shutdown, "Thread pool is on shutdown", ErrorCode::INTERNAL );

    LOG_TRACE_MSG( "" );

    m_queue.push( task );

    return ErrorCode::NONE;
}

}  // namespace common
}  // namespace uni

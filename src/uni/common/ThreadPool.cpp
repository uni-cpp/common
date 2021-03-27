////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/ThreadPool.cpp
/// @brief Implementation thread pool class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 09.2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "uni/common/ThreadPool.hpp"

namespace uni
{
namespace common
{
ThreadPool::ThreadPool( )
    : m_is_done( false )
{
    const size_t thread_count = std::thread::hardware_concurrency( );
    for( unsigned i = 0; i < thread_count; ++i )
    {
        m_threads.emplace_back( &ThreadPool::worker, this );
    }
}

ThreadPool::~ThreadPool( )
{
    m_is_done = true;

    for( auto& thread : m_threads )
    {
        if( thread.joinable( ) )
        {
            thread.join( );
        }
    }
}

void
ThreadPool::submit( std::function< void( ) >& f )
{
    std::lock_guard< std::mutex > lock{ m_mutex };
    m_queue.push( f );
}

void
ThreadPool::worker( )
{
    while( !m_is_done )
    {
        std::unique_lock< std::mutex > lock{ m_mutex };
        if( !m_queue.empty( ) )
        {
            std::function< void( ) > task = m_queue.front( );
            m_queue.pop( );
            task( );
        }
        else
        {
            lock.unlock( );
            std::this_thread::yield( );
            // Could be sleeped for ~50MS
        }
    }
}

}  // namespace common
}  // namespace uni

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
    : m_done( false )
{
    const size_t thread_count = std::thread::hardware_concurrency( );
    for( unsigned i = 0; i < thread_count; ++i )
    {
        m_threads.push_back( std::thread( &ThreadPool::worker_thread, this ) );
    }
}

ThreadPool::~ThreadPool( )
{
    m_done = true;

    for( auto& thread : m_threads )
    {
        if( thread.joinable( ) )
        {
            thread.join( );
        }
    }
}

void
ThreadPool::worker_thread( )
{
    while( !m_done )
    {
        if( !m_queue.empty( ) )
        {
            std::function< void( ) > task = m_queue.front( );
            m_queue.pop( );
            task( );
        }
        else
        {
            std::this_thread::yield( );
        }
    }
}

}  // namespace common
}  // namespace uni

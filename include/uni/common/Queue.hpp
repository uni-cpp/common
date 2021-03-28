////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/Queue.hpp
/// @brief Declaration thread safe queue class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 09.2020
/// @note Thanks to the "C++ Concurrency in Action" by Anthony Williams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uni/common/Defines.hpp"

#include <condition_variable>
#include <mutex>
#include <queue>

namespace uni
{
namespace common
{
template < class T >
class UNI_API Queue
{
    using DataType = T;

public:
    Queue( ) = default;

    void
    push( T data )
    {
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            m_queue.push( std::move( data ) );
        }
        m_cv.notify_one( );
    }

    void
    wait_and_pop( T& value )
    {
        std::unique_lock< std::mutex > lock( m_mutex );
        m_cv.wait( lock, [this] { return !m_queue.empty( ); } );
        value = std::move( m_queue.front( ) );
        m_queue.pop( );
    }

    std::shared_ptr< T >
    wait_and_pop( )
    {
        std::unique_lock< std::mutex > lock( m_mutex );
        m_cv.wait( lock, [this] { return !m_queue.empty( ); } );
        std::shared_ptr< T > res( std::make_shared< T >( std::move( m_queue.front( ) ) ) );
        m_queue.pop( );
        return res;
    }

    bool
    try_pop( T& value )
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        if( m_queue.empty( ) )
        {
            return false;
        }
        value = std::move( m_queue.front( ) );
        m_queue.pop( );
        return true;
    }

    std::shared_ptr< T >
    try_pop( )
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        if( m_queue.empty( ) )
        {
            return std::shared_ptr< T >( );
        }
        std::shared_ptr< T > res( std::make_shared< T >( std::move( m_queue.front( ) ) ) );
        m_queue.pop( );
        return res;
    }

    bool
    empty( ) const
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        return m_queue.empty( );
    }

private:
    mutable std::mutex m_mutex{};
    std::queue< T > m_queue{};
    std::condition_variable m_cv{};
};

}  // namespace common
}  // namespace uni

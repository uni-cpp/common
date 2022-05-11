////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/Queue.hpp
/// @brief Declaration thread safe queue class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2020-2022
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

enum class OperationStatus
{
    SUCCESS,
    UNSUCCESS,
    CLOSED
};

template < class T, typename Container = std::deque< T > >
class UNI_API Queue
{
    using DataType = T;

public:
    Queue( ) = default;

    void
    push( const T& data )
    {
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            if( m_is_closed )
            {
                return;
            }
            m_elements.push_front( std::move( data ) );
        }
        m_cv.notify_one( );
    }

    void
    push( T&& data )
    {
        {
            std::lock_guard< std::mutex > lock( m_mutex );
            if( m_is_closed )
            {
                return;
            }
            m_elements.push_front( std::move( data ) );
        }
        m_cv.notify_one( );
    }


    OperationStatus
    wait_pop( T& element )
    {
        std::unique_lock< std::mutex > lock( m_mutex );
        m_cv.wait( lock, [ this ]( ) { return is_not_empty_or_closed( ); } );
        if( empty( lock ) )
        {
            return OperationStatus::CLOSED;
        }
        element = std::move( m_elements.front( ) );
        m_elements.pop_front( );

        return OperationStatus::SUCCESS;
    }

    OperationStatus
    try_pop( T& value )
    {
        std::lock_guard< std::mutex > lock( m_mutex );
        if( m_elements.empty( ) )
        {
            return OperationStatus::UNSUCCESS;
        }

        value = std::move( m_elements.front( ) );
        m_elements.pop_front( );
        return OperationStatus::SUCCESS;
    }

    void
    close( )
    {
        {
            std::lock_guard< std::mutex > guard( m_mutex );
            m_is_closed = true;
        }
        m_cv.notify_all( );
    }

    bool
    closed( ) const
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        return m_is_closed;
    }

    bool
    empty( ) const
    {
        std::lock_guard< std::mutex > guard( m_mutex );
        return m_elements.empty( );
    }

private:
    bool
    empty( std::unique_lock< std::mutex >& /* m_mutex */ ) const noexcept
    {
        return m_elements.empty( );
    }

    bool
    is_not_empty_or_closed( ) const noexcept
    {
        return !m_elements.empty( ) || m_is_closed;
    }

private:
    Container m_elements{ };

    mutable std::mutex m_mutex{ };
    std::condition_variable m_cv{ };
    bool m_is_closed{ false };
};

}  // namespace common
}  // namespace uni

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/BaseListener.hpp
/// @brief Base listener's functionality.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 01.2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mutex>
#include <set>

namespace uni
{
namespace common
{
template < typename LISTENER >
class BaseNotifier
{
public:
    virtual ~BaseNotifier( )
    {
        std::lock_guard< std::mutex > lock{ _mutex };
        _listeners.clear( );
    };

    void
    add_listener( LISTENER* listener )
    {
        if( listener )
        {
            std::lock_guard< std::mutex > lock{ _mutex };
            _listeners.emplace( listener );
        }
    };

    void
    remove_listener( LISTENER* listener )
    {
        std::lock_guard< std::mutex > lock{ _mutex };
        _listeners.erase( listener );
    };

    template < typename CALLBACK, typename... ARGS >
    void
    notify_listeners( CALLBACK callback, ARGS&&... args ) const
    {
        std::lock_guard< std::mutex > lock{ _mutex };
        for( const auto& listener : _listeners )
        {
            ( listener->*callback )( args... );
        }
    }

private:
    mutable std::mutex _mutex;
    std::set< LISTENER* > _listeners;
};

}  // namespace common
}  // namespace uni

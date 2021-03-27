////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/BaseNotifier.hpp
/// @brief Base listener's functionality.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mutex>
#include <set>

namespace uni
{
namespace common
{
template < class ListenerT >
class BaseNotifier
{
public:
    virtual ~BaseNotifier( );

public:
    void add_listener( ListenerT* listener );
    void remove_listener( ListenerT* listener );

    template < class CallbackT, class... ARGS >
    void
    notify_listeners( CallbackT callback, ARGS&&... args ) const
    {
        std::lock_guard< std::mutex > lock{ m_mutex };
        for( const auto& listener : m_listeners )
        {
            ( listener->*callback )( std::forward( args... ) );
        }
    }

private:
    mutable std::mutex m_mutex;
    std::set< ListenerT* > m_listeners;
};

}  // namespace common
}  // namespace uni

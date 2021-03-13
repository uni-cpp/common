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
template < typename LISTENER >
class BaseNotifier
{
public:
    virtual ~BaseNotifier( );

public:
    void add_listener( LISTENER* listener );
    void remove_listener( LISTENER* listener );

    template < typename CALLBACK, typename... ARGS >
    void
    notify_listeners( CALLBACK callback, ARGS&&... args ) const
    {
        std::lock_guard< std::mutex > lock{ m__mutex };
        for( const auto& listener : m_listeners )
        {
            ( listener->*callback )( std::forward( args... ) );
        }
    }

private:
    mutable std::mutex m__mutex;
    std::set< LISTENER* > m_listeners;
};

}  // namespace common
}  // namespace uni

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/BaseNotifier.cpp
/// @brief Base listener's functionality.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <uni/common/BaseNotifier.hpp>

namespace uni
{
namespace common
{
template < class ListenerT >
BaseNotifier< ListenerT >::~BaseNotifier( )
{
    std::lock_guard< std::mutex > lock{ m_mutex };
    m_listeners.clear( );
};

template < class ListenerT >
void
BaseNotifier< ListenerT >::add_listener( ListenerT* listener )
{
    if( listener )
    {
        std::lock_guard< std::mutex > lock{ m_mutex };
        m_listeners.emplace( listener );
    }
};

template < class ListenerT >
void
BaseNotifier< ListenerT >::remove_listener( ListenerT* listener )
{
    std::lock_guard< std::mutex > lock{ m_mutex };
    m_listeners.erase( listener );
};

}  // namespace common
}  // namespace uni

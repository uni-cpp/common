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
template < typename LISTENER >
BaseNotifier< LISTENER >::~BaseNotifier( )
{
    std::lock_guard< std::mutex > lock{ _mutex };
    _listeners.clear( );
};

template < typename LISTENER >
void
BaseNotifier< LISTENER >::add_listener( LISTENER* listener )
{
    if( listener )
    {
        std::lock_guard< std::mutex > lock{ _mutex };
        _listeners.emplace( listener );
    }
};

template < typename LISTENER >
void
BaseNotifier< LISTENER >::remove_listener( LISTENER* listener )
{
    std::lock_guard< std::mutex > lock{ _mutex };
    _listeners.erase( listener );
};

}  // namespace common
}  // namespace uni

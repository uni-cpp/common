////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/Broadcast.hpp
/// @brief Declaration Broadcast class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <uni/common/Log.hpp>

#include <map>
#include <vector>

namespace uni
{
namespace common
{
namespace
{
template < class D >
size_t
get_class_id( )
{
    static const size_t hash = typeid( D ).hash_code( );
    return hash;
}
}  // namespace


// ================================================= 1 =================================================
class IEvent
{
public:
    virtual ~IEvent( ) = default;

    virtual size_t get_class_id( ) const = 0;
};

class IEventListener
{
public:
    virtual ~IEventListener( ) = default;

    virtual size_t get_class_id( ) const = 0;
    virtual void process_event( const IEvent& event ) = 0;
};


class IEventSender
{
public:
    virtual ~IEventSender( ) = default;

    virtual void send( std::shared_ptr< const IEvent >& event ) = 0;
};
// ================================================= ~1 =================================================

// ================================================= 2 =================================================
class IEventDispatcher
{
    using Listeners = std::vector< IEventListener* >;
    using ListenerMap = std::map< size_t, Listeners >;

public:
    virtual ~IEventDispatcher( ) = default;

    void dispatch( const IEvent& event );
    void register_listener( IEventListener* listener );
    void unregister_listener( IEventListener* listener );

protected:
    bool get_listeners( size_t class_id, Listeners& listeners );

protected:
    ListenerMap m_listeners{};
    std::mutex m_listeners_mutex;
};
// ================================================= ~2 =================================================

// ================================================= 3 =================================================
template < class D >
class Event : public IEvent
{
public:
    Event( const D& in_data = D( ) )
        : m_data( in_data )
    {
    }

    Event( const Event& o )
        : m_data( o.m_data )
    {
    }

    ~Event( ) override = default;

    size_t
    get_class_id( ) const override
    {
        return get_class_id< D >( );
    }

    static void
    broadcast_event( const D& data, IEventSender& sender )
    {
        std::shared_ptr< const IEvent > ev = std::make_shared< const Event< D > >( Event< D >( data ) );
        sender.send( ev );
    }

    const D&
    get_data( ) const
    {
        return m_data;
    }

public:
    D m_data;
};

template < class D >
class EventListener : public IEventListener
{
public:
    IEventDispatcher* m_p_dispatcher = nullptr;

public:
    EventListener( )
        : m_p_dispatcher( nullptr )
    {
    }

    ~EventListener( ) override
    {
        if( m_p_dispatcher )
        {
            m_p_dispatcher->unregister_listener( this );
            m_p_dispatcher = nullptr;
        }
    }

    size_t
    get_class_id( ) const override
    {
        return get_class_id< D >( );
    }

    virtual void process_event( const Event< D >& event ) = 0;

    void
    process_event( const IEvent& event )
    {
        if( event.get_class_id( ) == get_class_id( ) )
        {
            process_event( static_cast< const Event< D >& >( event ) );
        }
    }

    void
    register_listener( IEventDispatcher& disp )
    {
        if( !m_p_dispatcher )
        {
            disp.register_listener( this );
            m_p_dispatcher = &disp;
        }
        else
        {
            /// TODO: debug messages not available in headers yet.
        }
    }

    void
    unregister_listener( )
    {
        if( m_p_dispatcher )
        {
            unregister_listener( *m_p_dispatcher );
        }
    }

    void
    unregister_listener( IEventDispatcher& disp )
    {
        if( m_p_dispatcher )
        {
            disp.unregister_listener( this );
            m_p_dispatcher = nullptr;
        }
    }
};
// ================================================= ~3 =================================================

// ================================================= 4 =================================================
template < class _BroadcastDataType >
class Broadcast
{
public:
    using BroadcastDataType = _BroadcastDataType;

    class Receiver : public EventListener< BroadcastDataType >
    {
    public:
        Receiver( IEventDispatcher* dispatcher )
            : m_dispatcher( dispatcher )
        {
            if( !m_dispatcher )
            {
                LOG_FATAL_MSG( "Empty dispatcher" );
                return;
            }

            EventListener< BroadcastDataType >::register_listener( *m_dispatcher );
        }

        virtual ~Receiver( )
        {
            if( m_dispatcher )
            {
                EventListener< BroadcastDataType >::unregister_listener( *m_dispatcher );
            }

            m_dispatcher = nullptr;
        }

        virtual void handle_notification( const BroadcastDataType& arg ) = 0;

        void
        process_event( const Event< BroadcastDataType >& event )
        {
            handle_notification( event.get_data( ) );
        }

        IEventDispatcher* m_dispatcher;
    };

    class Sender
    {
    public:
        Sender( IEventSender* sender )
            : m_sender( sender )
        {
            if( !m_sender )
            {
                LOG_FATAL_MSG( "Empty sender" );
            }
        }

        virtual ~Sender( )
        {
            m_sender = nullptr;
        }

        void
        notify_all( const BroadcastDataType& event )
        {
            REQUIRED( m_sender, "Empty sender" );
            Event< BroadcastDataType >::broadcast_event( event, *m_sender );
        }

    private:
        IEventSender* m_sender;
    };
};
// ================================================= ~4 =================================================

// ================================================= 5 =================================================
// Exampl event struct. Should be in the user code
struct SampleEvent
{
    // useful data event
    bool is_activated{ true };
    std::string test_data{ "Hello, world" };

    //
    using Broadcast = Broadcast< SampleEvent >;

    // Logging
    LOG_CLASS( SampleEvent, LOG_IT( test_data ) );
};
// ================================================= ~5 =================================================

}  // namespace common
}  // namespace uni

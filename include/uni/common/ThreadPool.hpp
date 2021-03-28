////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/ThreadPool.hpp
/// @brief Declaration thread pool class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 2020-2021
/// @note Thanks to the "C++ Concurrency in Action" by Anthony Williams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uni/common/Defines.hpp"
#include "uni/common/Queue.hpp"
#include "uni/common/Thread.hpp"

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace uni
{
namespace common
{
using DefaultVoidStdFunction = std::function< void( ) >;

/*
 * Desc
 */
class UNI_API ThreadPool
{
public:
    struct Settings
    {
        Thread::Settings thread_settings{};
        uint32_t thread_count{ std::thread::hardware_concurrency( ) };

        LOG_CLASS( Settings, LOG_IT( thread_settings ), LOG_IT( thread_count ) );
    };

public:
    ThreadPool( const Settings& settings );
    ~ThreadPool( );

    /// Add new task to the queue
    ErrorCode submit( const DefaultVoidStdFunction& task );

private:
    std::atomic< bool > m_is_on_shutdown{ false };
    Queue< DefaultVoidStdFunction > m_queue{};
    std::vector< std::unique_ptr< uni::common::Thread > > m_threads{};
};

}  // namespace common
}  // namespace uni

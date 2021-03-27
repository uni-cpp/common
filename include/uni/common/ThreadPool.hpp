////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file uni/common/ThreadPool.hpp
/// @brief Declaration thread pool class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 09.2020
/// @note Thanks to the "C++ Concurrency in Action" by Anthony Williams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uni/common/Defines.hpp"

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
class UNI_API ThreadPool
{
public:
    ThreadPool( );
    ~ThreadPool( );

    void submit( std::function< void( ) >& f );

private:
    void worker( );

private:
    std::mutex m_mutex{};
    std::atomic< bool > m_is_done{ false };
    std::queue< std::function< void( ) > > m_queue{};

    std::vector< std::thread > m_threads{};
};

}  // namespace common
}  // namespace uni

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file app/main.cpp
/// @brief Redundant calculations of crc sums for data blocks in multiple threads.
/// @author Sergey Polyakov <white.irbys@gmail.com>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <crc/manager/Manager.hpp>

int main(int /*argc*/, char** /*argv*/)
{
    int retval = -1;
    // crc::config config
    // if (config.load(argc, argv))
    constexpr uint32_t nGeneratedThreads = 5;
    constexpr uint32_t nBlocksPerThread = 1000;
    constexpr uint32_t blockSize_byte = 100000;

    constexpr uint32_t nThreadsCalculateCrc = 10;
    crc::manager::Manager threadManager(nGeneratedThreads, nBlocksPerThread, blockSize_byte, nThreadsCalculateCrc);
    if (threadManager.run())
    {
        retval = 0;
    }

    return retval;
}

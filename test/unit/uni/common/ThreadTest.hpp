////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file test/uni/common/ThreadTest.hpp
/// @brief Declaration thread test class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 02.2021
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <uni/common/Thread.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test
{
namespace uni
{
namespace common
{
class ThreadTest
    : public ::uni::common::Thread
    , public testing::Test
{
    using Base = testing::Test;

public:
    ThreadTest( );
    ~ThreadTest( ) override = default;

    // Test
private:
    void SetUp( ) override;
    void TearDown( ) override;

    // Thread
private:
    void run( ) override;
};

}  // namespace common
}  // namespace uni
}  // namespace test

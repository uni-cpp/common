////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file test/uni/common/ThreadTest.cpp
/// @brief Implementation thread test class.
/// @author Sergey Polyakov <white.irbys@gmail.com>
/// @date 02.2022
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ThreadTest.hpp"

namespace
{
const std::string NAME_TEST_THREAD{ "TEST_MainThread" };
}  // namespace

namespace test
{
namespace uni
{
namespace common
{
ThreadTest::ThreadTest( )
    : ::uni::common::Thread( NAME_TEST_THREAD )
{
}

void
ThreadTest::SetUp( )
{
    ASSERT_NO_FATAL_FAILURE( Base::SetUp( ) );
}

void
ThreadTest::TearDown( )
{
    ASSERT_NO_FATAL_FAILURE( Base::TearDown( ) );
}

void
ThreadTest::run( )
{
    return;
}

TEST_F( ThreadTest, SimpleCtorDtor )
{
    ASSERT_TRUE( true );
}

TEST_F( ThreadTest, WithoutStart )
{
    ASSERT_FALSE( is_running( ) );
    ASSERT_FALSE( is_on_thread( ) );
}

TEST_F( ThreadTest, StartWithoutStop )
{
    ASSERT_TRUE( start( ) );
    ASSERT_TRUE( is_running( ) );
}

TEST_F( ThreadTest, StartStop )
{
    ASSERT_TRUE( start( ) );
    ASSERT_TRUE( is_running( ) );
    ASSERT_TRUE( stop( ) );
    ASSERT_FALSE( is_running( ) );
}

TEST_F( ThreadTest, MultipleStartStop )
{
    ASSERT_TRUE( start( ) );
    ASSERT_TRUE( is_running( ) );
    ASSERT_TRUE( stop( ) );
    ASSERT_FALSE( is_running( ) );

    ASSERT_TRUE( start( ) );
    ASSERT_TRUE( is_running( ) );
    ASSERT_TRUE( stop( ) );
    ASSERT_FALSE( is_running( ) );
}

TEST_F( ThreadTest, StopWithoutStart )
{
    ASSERT_FALSE( stop( ) );

    ASSERT_TRUE( start( ) );
    ASSERT_TRUE( stop( ) );

    ASSERT_FALSE( stop( ) );
}

TEST_F( ThreadTest, DoubleStart )
{
    ASSERT_TRUE( start( ) );
    ASSERT_FALSE( start( ) );
    ASSERT_TRUE( stop( ) );
}

TEST_F( ThreadTest, GetName )
{
    ASSERT_EQ( NAME_TEST_THREAD, get_name( ) );
}

}  // namespace common
}  // namespace uni
}  // namespace test

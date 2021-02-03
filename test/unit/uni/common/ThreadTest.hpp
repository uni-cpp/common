#include <gmock/gmock.h>
#include <gtest/gtest.h>

class ThreadTest : public testing::Test
{
    using Base = testing::Test;

public:
    ThreadTest( )
        : testing::Test( )
    {
    }

protected:
    void
    SetUp( ) override
    {
        ASSERT_NO_FATAL_FAILURE( Base::SetUp( ) );
    }

    void
    TearDown( ) override
    {
        ASSERT_NO_FATAL_FAILURE( Base::TearDown( ) );
    }
};

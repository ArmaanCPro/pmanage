#include <gtest/gtest.h>
/*
#include <expected>
#include <memory>
#include <string>
*/

import pmanage;

TEST(Pid1CanBeFound, BasicAssertions)
{
    auto process = pmng::process::find_by_pid(1);

    EXPECT_EQ(process.has_value(), true);
}

TEST(Pid0CannotBeFound, BasicAssertions)
{
    auto process = pmng::process::find_by_pid(0);

    EXPECT_EQ(process.has_value(), true);
}

TEST(HasCurrentPid, BasicAssertions)
{
    auto current_pid = pmng::process::current_pid();

    EXPECT_NE(current_pid, 0);
}

TEST(CurrentPidMatchesCurrentPidByName, BasicAssertions)
{
    auto current_pid = pmng::process::current_pid();
    auto proc = pmng::process::find_by_name("pmanageTests");

    EXPECT_TRUE(proc.has_value()) << "Could not find current process by name: " << proc.error();

    if (proc.has_value())
    {
        EXPECT_EQ(proc.value()->pid(), current_pid);
    }
}


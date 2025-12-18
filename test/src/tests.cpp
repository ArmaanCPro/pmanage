#include <gtest/gtest.h>

import pmanage;

TEST(Pid1CanBeFound, BasicAssertions)
{
    auto process = pmng::process::find_by_pid(1);

    EXPECT_EQ(process.has_value(), true);
}

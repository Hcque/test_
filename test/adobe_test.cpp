
#include <gtest/gtest.h>
#include <iostream>

#include "adobe/block_access_controller.h"

namespace mynoise {

TEST(HelloTest, BasicAssertion)
{
    std::cout << "START Test " << std::endl;
    auto b = BlockAccessController();



    EXPECT_STRNE("hello", "word");
    EXPECT_EQ(6*7, 42);

}

} // namespace mynoise
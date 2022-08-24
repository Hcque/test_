
#include <gtest/gtest.h>
#include <iostream>

TEST(HelloTest, BasicAssertion)
{
    std::cout << "START Test " << std::endl;
    EXPECT_STRNE("hello", "word");
    EXPECT_EQ(6*7, 42);

}
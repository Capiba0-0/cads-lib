#include <gtest/gtest.h>

#include "cads/stack.h"

TEST(StackTest, LIFO_Behaviour)
{
    cads::Stack<int> stack;

    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);

    stack.push(10);
    stack.push(20);
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.size(), 2);

    EXPECT_EQ(stack.top(), 20);

    stack.pop();
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 10);

    stack.pop();
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
}
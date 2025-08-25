#include "gtest/gtest.h"
#include "cads/queue.h"

TEST(QueueTest, FIFO_Behaviour)
{
    cads::Queue<int> queue;

    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);

    queue.push(10);
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 1);

    EXPECT_EQ(queue.back(), 10);
    EXPECT_EQ(queue.front(), 10);

    queue.push(20);
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 2);

    EXPECT_EQ(queue.back(), 20);
    EXPECT_EQ(queue.front(), 10);

    queue.pop();
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front(), 20);

    queue.pop();
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
}
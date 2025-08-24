#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "cads/list.h"

// --- HELPERS ---
struct InstanceCounter {
    static inline int liveInstances = 0;

    InstanceCounter() {
        liveInstances++;
    }

    InstanceCounter(const InstanceCounter&) {
        liveInstances++;
    }
    InstanceCounter(InstanceCounter&&) noexcept {
        liveInstances++;
    }

    ~InstanceCounter() {
        liveInstances--;
    }

    InstanceCounter& operator=(const InstanceCounter&) = default;
    InstanceCounter& operator=(InstanceCounter&&) noexcept = default;
};

// --- TESTS ---
// ListTest
TEST(ListTest, DefaultConstructor)
{
    cads::List<int> list;

    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, SizeAndValueConstructor)
{
    cads::List list(10, 5);
    EXPECT_EQ(list.size(), 10);

    for (const auto& item : list)
        EXPECT_EQ(item, 5);

    cads::List<int> list2(10);
    EXPECT_EQ(list2.size(), 10);

    for (const auto& item : list2)
        EXPECT_EQ(item, 0);
}

// ListAccessTest
TEST(ListAccessTest, FrontAndBack)
{
    cads::List list { 10 };
    EXPECT_EQ(list.front(), 10);
    EXPECT_EQ(list.back(), 10);

    cads::List list2 { 10, 20, 30 };
    EXPECT_EQ(list2.front(), 10);
    EXPECT_EQ(list2.back(), 30);

    list2.front() = 199;
    list2.back() = 299;
    EXPECT_EQ(*list2.begin(), 199);
    EXPECT_EQ(*list2.rbegin(), 299);
}

// ListIteratorTest
TEST(ListIteratorTest, DefaultIteratorMethods)
{
    cads::List list { 10, 20, 30, 40, 50 };

    EXPECT_EQ(*list.begin(), 10);
    EXPECT_EQ(*(--list.end()), 50);
    EXPECT_EQ(*list.rbegin(), 50);
    EXPECT_EQ(*(--list.rend()), 10);

    *list.begin() = 199;
    *list.rbegin() = 599;
    EXPECT_EQ(list.front(), 199);
    EXPECT_EQ(list.back(), 599);

    *(--list.end()) = 600;
    *(--list.rend()) = 200;
    EXPECT_EQ(list.front(), 200);
    EXPECT_EQ(list.back(), 600);
}

TEST(ListIteratorTest, ConstIteratorMethods)
{
    const cads::List list { 10, 20, 30, 40, 50 };

    EXPECT_EQ(*list.cbegin(), 10);
    EXPECT_EQ(*(--list.cend()), 50);
    EXPECT_EQ(*list.crbegin(), 50);
    EXPECT_EQ(*(--list.crend()), 10);
}

// ListSizeTest
TEST(ListSizeTest, Size)
{
    const cads::List<int> list;
    EXPECT_EQ(list.size(), 0);

    cads::List list2 { 10, 20, 30 };
    EXPECT_EQ(list2.size(), 3);

    list2.clear();
    EXPECT_EQ(list2.size(), 0);
}

TEST(ListSizeTest, Empty)
{
    const cads::List<int> list;
    ASSERT_TRUE(list.empty());

    const cads::List<int> list2 { 10, 20, 30 };
    ASSERT_FALSE(list2.empty());
}

// ListModifiersTest
TEST(ListModifiersTest, PushBack)
{
    cads::List<int> list;

    list.pushBack(10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.back(), 10);

    list.pushBack(20);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 20);

    list.pushBack(30);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.back(), 30);
}

TEST(ListModifiersTest, PushFront)
{
    cads::List<int> list;

    list.pushFront(10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 10);

    list.pushFront(20);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 20);

    list.pushFront(30);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 30);
}

TEST(ListModifiersTest, PopBack)
{
    cads::List list { 10, 20, 30 };

    list.popBack();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 10);
    EXPECT_EQ(list.back(), 20);
}

TEST(ListModifiersTest, PopFront)
{
    cads::List list { 10, 20, 30 };

    list.popFront();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 20);
    EXPECT_EQ(list.back(), 30);
}

TEST(ListModifiersTest, Clear)
{
    cads::List list{ 10, 20, 30 };
    ASSERT_EQ(list.size(), 3);

    list.clear();
    ASSERT_EQ(list.size(), 0);
}

TEST(ListModifiersTest, ClearEmpty)
{
    cads::List<int> list;
    ASSERT_EQ(list.size(), 0);

    list.clear();
    ASSERT_EQ(list.size(), 0);
}

TEST(ListModifiersTest, Swap)
{
    cads::List list { 1, 2 };
    cads::List list2 { 3, 4, 5 };

    list.swap(list2);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list2.size(), 2);

    EXPECT_EQ(list.front(), 3);
    EXPECT_EQ(list.back(), 5);
    EXPECT_EQ(list2.front(), 1);
    EXPECT_EQ(list2.back(), 2);
}

TEST(ListModifiersTest, SelfSwap)
{
    cads::List list { 1, 2 };

    list.swap(list);
    EXPECT_EQ(list.size(), 2);

    EXPECT_EQ(*list.begin(), 1);
    EXPECT_EQ(*list.rbegin(), 2);
}

// ListMemoryTest
TEST(ListMemoryTest, InitializerListAndDestructor)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        const cads::List<InstanceCounter> list = { {}, {}, {} };

        ASSERT_EQ(list.size(), 3);
        ASSERT_EQ(InstanceCounter::liveInstances, 4); // +1 for sentinel
    }

    EXPECT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(ListMemoryTest, CopyConstructor)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        const cads::List<InstanceCounter> original{ {}, {} };
        ASSERT_EQ(InstanceCounter::liveInstances, 3); // +1 for sentinel

        const cads::List<InstanceCounter> copy{ original };
        ASSERT_EQ(InstanceCounter::liveInstances, 6);
    }

    EXPECT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(ListMemoryTest, CopyAssignmentOperator)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::List<InstanceCounter> list1 { {}, {} };
        cads::List<InstanceCounter> list2 { {}, {}, {} };
        ASSERT_EQ(InstanceCounter::liveInstances, 7); // +2 for sentinels

        list1 = list2;

        ASSERT_EQ(list1.size(), 3);
        ASSERT_EQ(list2.size(), 3);
        ASSERT_EQ(InstanceCounter::liveInstances, 8);
    }

    EXPECT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(ListMemoryTest, MoveConstructor)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::List<InstanceCounter> original{ {}, {} };
        ASSERT_EQ(InstanceCounter::liveInstances, 3); // +1 for sentinel

        const cads::List<InstanceCounter> copy{ std::move(original) };
        ASSERT_EQ(InstanceCounter::liveInstances, 4); // +1 for sentinel of `original`
    }

    EXPECT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(ListMemoryTest, MoveAssignmentOperator)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::List<InstanceCounter> list1 { {}, {} };
        cads::List<InstanceCounter> list2 { {}, {}, {} };
        ASSERT_EQ(InstanceCounter::liveInstances, 7); // +2 for sentinels

        list1 = std::move(list2);

        ASSERT_EQ(list1.size(), 3);
        ASSERT_EQ(InstanceCounter::liveInstances, 7); // copy-and-swap
    }

    EXPECT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(ListMemoryTest, PushBackLvalueAndRvalue)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::List<InstanceCounter> list;
        const InstanceCounter object;

        ASSERT_EQ(InstanceCounter::liveInstances, 2); // +1 for sentinel of `list`

        list.pushBack(object);
        ASSERT_EQ(InstanceCounter::liveInstances, 3);

        list.pushBack(InstanceCounter{});
        ASSERT_EQ(InstanceCounter::liveInstances, 4);
    }

    ASSERT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(ListMemoryTest, PushFrontLvalueAndRvalue)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::List<InstanceCounter> list;
        const InstanceCounter object;

        ASSERT_EQ(InstanceCounter::liveInstances, 2); // +1 for sentinel of `list`

        list.pushFront(object);
        ASSERT_EQ(InstanceCounter::liveInstances, 3);

        list.pushFront(InstanceCounter{});
        ASSERT_EQ(InstanceCounter::liveInstances, 4);
    }

    ASSERT_EQ(InstanceCounter::liveInstances, 0);
}

// ListInsertTest
class ListInsertTest : public ::testing::Test
{
protected:
    cads::List<int> list;
};

TEST_F(ListInsertTest, InsertsIntoEmptyList)
{
    auto it = list.insert(list.begin(), 10);

    EXPECT_EQ(list.size(), 1);
    ASSERT_NE(it, list.end()); // Testing for validate
    EXPECT_EQ(*it, 10);
    EXPECT_THAT(list, ::testing::ElementsAre(10));
}

TEST_F(ListInsertTest, InsertsAtBegin)
{
    list = { 20, 30 };

    auto it = list.insert(list.begin(), 10);

    EXPECT_EQ(list.size(), 3);
    ASSERT_NE(it, list.end()); // Testing for validate
    EXPECT_EQ(*it, 10);
    EXPECT_THAT(list, ::testing::ElementsAre(10, 20, 30));
}

TEST_F(ListInsertTest, InsertsAtEnd)
{
    list = { 10, 20 };

    auto it = list.insert(list.end(), 30);

    EXPECT_EQ(list.size(), 3);
    ASSERT_NE(it, list.end()); // Testing for validate
    EXPECT_EQ(*it, 30);
    EXPECT_THAT(list, ::testing::ElementsAre(10, 20, 30));
}

TEST_F(ListInsertTest, InsertsInMiddle)
{
    list = { 10, 30 };

    auto posIt = ++list.begin();

    auto insertedIt = list.insert(posIt, 20);

    EXPECT_EQ(list.size(), 3);
    ASSERT_NE(insertedIt, list.end()); // Testing for validate
    EXPECT_EQ(*insertedIt, 20);
    EXPECT_THAT(list, ::testing::ElementsAre(10, 20, 30));
}

TEST_F(ListInsertTest, ReturnsCorrectIterator)
{
    list = { 10, 30 };

    auto posIt = ++list.begin();

    auto returnedIt = list.insert(posIt, 20);

    EXPECT_EQ(*returnedIt, 20);

    auto checkIt = ++list.begin();
    EXPECT_EQ(returnedIt, checkIt);
}

// ListEraseTest
class ListEraseTest : public ::testing::Test
{
protected:
    cads::List<int> list;
    cads::List<int>::Iterator it;
};

TEST_F(ListEraseTest, OneElementAtBegin)
{
    list = { 10, 20, 30 };

    it = list.erase(list.begin());

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(*it, 20);
    EXPECT_THAT(list, ::testing::ElementsAre( 20, 30 ));
}

TEST_F(ListEraseTest, OneElementInside)
{
    list = { 10, 20, 30 };

    auto pos = list.begin();
    ++pos;

    it = list.erase(pos);

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(*it, 30);
    EXPECT_THAT(list, ::testing::ElementsAre( 10, 30 ));
}

TEST_F(ListEraseTest, OneElementAtEnd)
{
    list = { 10, 20, 30 };

    auto pos = list.end();
    --pos;

    it = list.erase(pos);

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(it, list.end());
    EXPECT_THAT(list, ::testing::ElementsAre( 10, 20 ));
}

TEST_F(ListEraseTest, RangeAtBegin)
{
    list = { 10, 20, 30, 40, 50 };

    auto firstIt = list.begin();
    auto lastIt = list.begin(); ++lastIt; ++lastIt;

    it = list.erase(firstIt, lastIt);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(*it, 30);
    EXPECT_THAT(list, ::testing::ElementsAre( 30, 40, 50 ));
}

TEST_F(ListEraseTest, RangeInside)
{
    list = { 10, 20, 30, 40, 50 };

    auto firstIt = list.begin(); ++firstIt;
    auto lastIt = list.begin(); ++lastIt; ++lastIt; ++lastIt; ++lastIt;

    it = list.erase(firstIt, lastIt);

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(*it, 50);
    EXPECT_THAT(list, ::testing::ElementsAre( 10, 50 ));
}

TEST_F(ListEraseTest, RangeAtEnd)
{
    list = { 10, 20, 30, 40, 50 };

    auto firstIt = list.begin(); ++firstIt; ++firstIt; ++firstIt;
    auto lastIt = list.end();

    it = list.erase(firstIt, lastIt);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(it, list.end());
    EXPECT_THAT(list, ::testing::ElementsAre( 10, 20, 30 ));
}

TEST_F(ListEraseTest, FullList)
{
    list = { 10, 20, 30, 40, 50 };

    it = list.erase(list.begin(), list.end());

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(it, list.end());
    EXPECT_EQ(list.begin(), list.end());
}

TEST_F(ListEraseTest, EmptyRange)
{
    list = { 10, 20, 30, 40, 50 };
    it = list.erase(list.begin(), list.begin());

    EXPECT_EQ(list.size(), 5);
    EXPECT_EQ(it, list.begin());
    EXPECT_THAT(list, ::testing::ElementsAre( 10, 20, 30, 40, 50 ));
}

// ListRemoveTest
class ListRemoveTest : public ::testing::Test
{
protected:
    cads::List<int> list;
};

TEST_F(ListRemoveTest, OneElementRemove)
{
    list = { 10, 20, 30, 40, 50 };

    list.remove(30);

    EXPECT_EQ(list.size(), 4);
    EXPECT_THAT(list, ::testing::ElementsAre( 10, 20, 40, 50 ));
}

TEST_F(ListRemoveTest, SeveralElementsRemove)
{
    list = { 10, 40, 20, 40, 30 };

    list.remove(40);

    EXPECT_EQ(list.size(), 3);
    EXPECT_THAT(list, ::testing::ElementsAre( 10, 20, 30 ));
}

TEST_F(ListRemoveTest, AllElementsRemove)
{
    list = { 10, 10, 10, 10, 10 };

    list.remove(10);

    EXPECT_EQ(list.size(), 0);
    EXPECT_EQ(list.begin(), list.end());
}

TEST_F(ListRemoveTest, NonExistentElementRemove)
{
    list = { 10, 20, 30, 40, 50 };

    list.remove(60);

    EXPECT_EQ(list.size(), 5);
    EXPECT_THAT(list, ::testing::ElementsAre( 10, 20, 30, 40, 50 ));
}


// ListReverseTest
class ListReverseTest : public ::testing::Test
{
protected:
    cads::List<int> list;
};

TEST_F(ListReverseTest, EmptyList)
{
    auto itBegin = list.begin();
    auto itEnd = list.end();

    list.reverse();

    EXPECT_EQ(list.size(), 0);
    EXPECT_EQ(list.begin(), list.end());
    EXPECT_EQ(list.begin(), itBegin);
    EXPECT_EQ(list.end(), itEnd);
}

TEST_F(ListReverseTest, SingleElementList)
{
    list = { 10 };
    auto itBegin = list.begin();
    auto itEnd = list.end();

    list.reverse();

    EXPECT_EQ(list.size(), 1);
    EXPECT_THAT(list, ::testing::ElementsAre( 10 ));
    EXPECT_EQ(list.begin(), itBegin);
    EXPECT_EQ(list.end(), itEnd);
}

TEST_F(ListReverseTest, MultipleElementsList)
{
    list = { 10, 20, 30 };

    list.reverse();

    EXPECT_EQ(list.size(), 3);
    EXPECT_THAT(list, ::testing::ElementsAre( 30, 20, 10 ));
}

// ListSpliceTest
class ListSpliceTest : public ::testing::Test
{
protected:
    cads::List<int> list1;
    cads::List<int> list2;
};

TEST_F(ListSpliceTest, FullList)
{
    list1 = { 10, 50, 60 };
    list2 = { 20, 30, 40 };

    auto posIt = list1.begin();
    ++posIt;

    list1.splice(posIt, list2, list2.begin(), list2.end());

    EXPECT_EQ(list1.size(), 6);
    EXPECT_EQ(list2.size(), 0);
    EXPECT_THAT(list1, ::testing::ElementsAre(10, 20, 30, 40, 50, 60));
}

TEST_F(ListSpliceTest, Range)
{
    list1 = { 10, 20, 30 };
    list2 = { 40, 50, 60 };

    auto posIt = list1.end();

    auto lastIt = list2.begin();
    ++lastIt; ++lastIt;

    list1.splice(posIt, list2, list2.begin(), lastIt);

    EXPECT_EQ(list1.size(), 5);
    EXPECT_EQ(list2.size(), 1);
    EXPECT_THAT(list1, ::testing::ElementsAre(10, 20, 30, 40, 50));
    EXPECT_THAT(list2, ::testing::ElementsAre(60));
}

TEST_F(ListSpliceTest, InsideSelf)
{
    list1 = { 30, 40, 50, 60, 10, 20 };

    auto firstIt = list1.begin();
    ++firstIt; ++firstIt; ++firstIt; ++firstIt;

    list1.splice(list1.begin(), list1, firstIt, list1.end());

    EXPECT_EQ(list1.size(), 6);
    EXPECT_THAT(list1, ::testing::ElementsAre(10, 20, 30, 40, 50, 60));
}
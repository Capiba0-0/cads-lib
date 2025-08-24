#include <gtest/gtest.h>
#include "cads/vector.h"

#include <stdexcept>

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
// VectorTest
TEST(VectorTest, DefaultConstructor)
{
    cads::Vector<int> v;

    EXPECT_EQ(v.data(), nullptr);

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, SizeAndValueConstructor)
{
    cads::Vector vec(10, 5);
    EXPECT_EQ(vec.size(), 10);
    EXPECT_EQ(vec.capacity(), 10);

    for (int i = 0; i < 10; i++)
        EXPECT_EQ(vec[i], 5);

    cads::Vector<int> vec2(10);
    EXPECT_EQ(vec2.size(), 10);
    EXPECT_EQ(vec2.capacity(), 10);

    for (int i = 0; i < 10; i++)
        EXPECT_EQ(vec2[i], 0);
}

// VectorAccessTest
TEST(VectorAccessTest, OperatorBrackets)
{
    cads::Vector vec { 10, 20, 30 };

    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);

    vec[0] = 99;
    EXPECT_EQ(vec[0], 99);
}

TEST(VectorAccessTest, AtMethodNormal)
{
    cads::Vector vec { 10, 20, 30 };

    EXPECT_EQ(vec.at(0), 10);
    EXPECT_EQ(vec.at(1), 20);
    EXPECT_EQ(vec.at(2), 30);

    vec.at(0) = 99;
    EXPECT_EQ(vec.at(0), 99);
}

TEST(VectorAccessTest, AtMethodThrow)
{
    cads::Vector vec { 10, 20, 30 };

    EXPECT_THROW(vec.at(20), std::out_of_range);
    EXPECT_THROW(vec.at(vec.size()), std::out_of_range);

    cads::Vector<int> emptyVec;

    EXPECT_THROW(emptyVec.at(0), std::out_of_range);
}

TEST(VectorAccessTest, FrontAndBack)
{
    cads::Vector vec { 10 };
    EXPECT_EQ(vec.front(), 10);
    EXPECT_EQ(vec.back(), 10);

    cads::Vector vec2 { 10, 20, 30 };
    EXPECT_EQ(vec2.front(), 10);
    EXPECT_EQ(vec2.back(), 30);

    vec2.front() = 199;
    vec2.back() = 299;
    EXPECT_EQ(vec2[0], 199);
    EXPECT_EQ(vec2[2], 299);
}

TEST(VectorAccessTest, DataMethod)
{
    cads::Vector<int> vec;
    EXPECT_EQ(vec.data(), nullptr);

    cads::Vector vec2 { 10, 20, 30 };

    auto* data = vec2.data();
    data[0] = 199;
    data[1] = 299;
    data[2] = 399;
    EXPECT_EQ(vec2[0], 199);
    EXPECT_EQ(vec2[1], 299);
    EXPECT_EQ(vec2[2], 399);
}

// VectorIteratorTest
TEST(VectorIteratorTest, DefaultIteratorMethods)
{
    cads::Vector vec { 10, 20, 30, 40, 50 };

    EXPECT_EQ(*vec.begin(), 10);
    EXPECT_EQ(*(--vec.end()), 50);
    EXPECT_EQ(*vec.rbegin(), 50);
    EXPECT_EQ(*(--vec.rend()), 10);

    *vec.begin() = 199;
    *vec.rbegin() = 599;
    EXPECT_EQ(vec[0], 199);
    EXPECT_EQ(vec[4], 599);

    *(--vec.end()) = 600;
    *(--vec.rend()) = 200;
    EXPECT_EQ(vec[0], 200);
    EXPECT_EQ(vec[4], 600);
}

TEST(VectorIteratorTest, ConstIteratorMethods)
{
    const cads::Vector vec { 10, 20, 30, 40, 50 };

    EXPECT_EQ(*vec.cbegin(), 10);
    EXPECT_EQ(*(--vec.cend()), 50);
    EXPECT_EQ(*vec.crbegin(), 50);
    EXPECT_EQ(*(--vec.crend()), 10);
}

// VectorCapacityTest
TEST(VectorCapacityTest, Size)
{
    const cads::Vector<int> vec;
    EXPECT_EQ(vec.size(), 0);

    cads::Vector vec2 { 10, 20, 30 };
    EXPECT_EQ(vec2.size(), 3);

    vec2.clear();
    EXPECT_EQ(vec2.size(), 0);
}

TEST(VectorCapacityTest, Capacity)
{
    const cads::Vector<int> vec;
    EXPECT_EQ(vec.capacity(), 0);

    cads::Vector vec2 { 10, 20, 30 };
    EXPECT_EQ(vec2.capacity(), 3);

    vec2.pushBack(40);
    EXPECT_EQ(vec2.capacity(), 6);
}

TEST(VectorCapacityTest, Empty)
{
    const cads::Vector<int> vec;
    ASSERT_TRUE(vec.empty());

    const cads::Vector<int> vec2 { 10, 20, 30 };
    ASSERT_FALSE(vec2.empty());
}

TEST(VectorCapacityTest, Reserve)
{
    cads::Vector vec{ 10, 20, 30 };
    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(vec.capacity(), 3);

    vec.reserve(10);
    ASSERT_EQ(vec.capacity(), 10);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);

    vec.pushBack(40);
    ASSERT_EQ(vec.size(), 4);
    ASSERT_EQ(vec.capacity(), 10);

    vec.reserve(5);
    ASSERT_EQ(vec.capacity(), 10);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
}

TEST(VectorCapacityTest, Resize)
{
    cads::Vector vec{ 10, 20, 30 };

    vec.resize(5);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.capacity(), 5);

    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
    EXPECT_EQ(vec[3], 0);
    EXPECT_EQ(vec[4], 0);

    vec.resize(2);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.capacity(), 5);

    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);

    vec.resize(3, 99);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 5);

    EXPECT_EQ(vec[2], 99);
}

TEST(VectorCapacityTest, ShrinkToFit)
{
    cads::Vector vec{ 10, 20, 30, 40, 50 };

    vec.resize(3);
    ASSERT_EQ(vec.capacity(), 5);

    vec.shrinkToFit();
    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(vec.capacity(), 3);
}

// VectorModifiersTest
TEST(VectorModifiersTest, Insert)
{
    cads::Vector<int> vec;

    cads::Vector<int>::Iterator it;
    it = vec.insert(vec.begin(), 10);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.capacity(), 1);
    EXPECT_EQ(it, vec.begin());

    EXPECT_EQ(vec[0], 10);

    it = vec.insert(vec.end(), 20);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.capacity(), 2);
    EXPECT_EQ(it, vec.begin() + 1);

    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);

    it = vec.insert(vec.begin(), 30);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 4);
    EXPECT_EQ(it, vec.begin());

    EXPECT_EQ(vec[0], 30);
    EXPECT_EQ(vec[1], 10);
    EXPECT_EQ(vec[2], 20);

    it = vec.insert(vec.begin() + 2, 40);
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec.capacity(), 4);
    EXPECT_EQ(it, vec.begin() + 2);

    EXPECT_EQ(vec[0], 30);
    EXPECT_EQ(vec[1], 10);
    EXPECT_EQ(vec[2], 40);
    EXPECT_EQ(vec[3], 20);

    it = vec.insert(vec.begin() + 1, 50);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.capacity(), 8);
    EXPECT_EQ(it, vec.begin() + 1);

    EXPECT_EQ(vec[0], 30);
    EXPECT_EQ(vec[1], 50);
    EXPECT_EQ(vec[2], 10);
    EXPECT_EQ(vec[3], 40);
    EXPECT_EQ(vec[4], 20);
}

TEST(VectorModifiersTest, PushBack)
{
    cads::Vector<int> vec;

    vec.pushBack(10);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.capacity(), 1);

    vec.pushBack(20);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.capacity(), 2);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);

    vec.pushBack(30);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 4); // 2 * 2 = 4
    EXPECT_EQ(vec[2], 30);
}

TEST(VectorModifiersTest, PopBack)
{
    cads::Vector vec { 10, 20, 30 };

    vec.popBack();
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.capacity(), 3);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
}

TEST(VectorModifiersTest, Clear)
{
    cads::Vector vec{ 10, 20, 30 };
    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(vec.capacity(), 3);

    vec.clear();
    ASSERT_EQ(vec.size(), 0);
    ASSERT_EQ(vec.capacity(), 3);
}

TEST(VectorModifiersTest, ClearEmpty)
{
    cads::Vector<int> vec;
    ASSERT_EQ(vec.size(), 0);

    vec.clear();
    ASSERT_EQ(vec.size(), 0);
}

TEST(VectorModifiersTest, EraseOneElement)
{
    cads::Vector vec{ 10, 20, 30, 40, 50 };

    cads::Vector<int>::Iterator it;

    it = vec.erase(vec.begin());
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec.capacity(), 5);
    EXPECT_EQ(it, vec.begin());

    EXPECT_EQ(vec[0], 20);
    EXPECT_EQ(vec[1], 30);
    EXPECT_EQ(vec[2], 40);
    EXPECT_EQ(vec[3], 50);

    it = vec.erase(vec.end() - 1);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 5);
    EXPECT_EQ(it, vec.end());

    EXPECT_EQ(vec[0], 20);
    EXPECT_EQ(vec[1], 30);
    EXPECT_EQ(vec[2], 40);

    it = vec.erase(vec.begin() + 1);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.capacity(), 5);
    EXPECT_EQ(it, vec.begin() + 1);

    EXPECT_EQ(vec[0], 20);
    EXPECT_EQ(vec[1], 40);
}

TEST(VectorModifiersTest, EraseElementRange)
{
    cads::Vector vec{ 10, 20, 30, 40, 50 };

    cads::Vector<int>::Iterator it;

    it = vec.erase(vec.begin() + 1, vec.begin() + 3);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 5);
    EXPECT_EQ(it, vec.begin() + 1);

    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 40);
    EXPECT_EQ(vec[2], 50);

    it = vec.erase(vec.begin(), vec.end());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 5);
    EXPECT_EQ(it, vec.end());

    it = vec.erase(vec.begin(), vec.begin());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 5);
    EXPECT_EQ(it, vec.end());
}

TEST(VectorModifiersTest, Swap)
{
    cads::Vector vec { 1, 2 };
    cads::Vector vec2 { 3, 4, 5 };

    vec.swap(vec2);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec2.size(), 2);

    EXPECT_EQ(vec.capacity(), 3);
    EXPECT_EQ(vec2.capacity(), 2);

    EXPECT_EQ(vec[0], 3);
    EXPECT_EQ(vec[1], 4);
    EXPECT_EQ(vec[2], 5);
    EXPECT_EQ(vec2[0], 1);
    EXPECT_EQ(vec2[1], 2);
}

TEST(VectorModifiersTest, SelfSwap)
{
    cads::Vector vec { 1, 2 };

    vec.swap(vec);
    EXPECT_EQ(vec.size(), 2);

    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
}

// VectorMemoryTest
TEST(VectorMemoryTest, InitializerListAndDestructor)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::Vector<InstanceCounter> v = { {}, {}, {} };

        ASSERT_EQ(v.size(), 3);
        ASSERT_EQ(InstanceCounter::liveInstances, 3);
    }

    EXPECT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(VectorMemoryTest, CopyConstructor)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        const cads::Vector<InstanceCounter> original{ {}, {} };
        ASSERT_EQ(InstanceCounter::liveInstances, 2);

        const cads::Vector<InstanceCounter> copy{ original };
        ASSERT_EQ(InstanceCounter::liveInstances, 4);
    }

    EXPECT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(VectorMemoryTest, CopyAssignmentOperator)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::Vector<InstanceCounter> v1 { {}, {} };
        cads::Vector<InstanceCounter> v2 { {}, {}, {} };
        ASSERT_EQ(InstanceCounter::liveInstances, 5);

        v1 = v2;

        ASSERT_EQ(v1.size(), 3);
        ASSERT_EQ(v2.size(), 3);
        ASSERT_EQ(InstanceCounter::liveInstances, 6);
    }

    EXPECT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(VectorMemoryTest, MoveConstructor)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::Vector<InstanceCounter> original{ {}, {} };
        ASSERT_EQ(InstanceCounter::liveInstances, 2);

        const cads::Vector<InstanceCounter> copy{ std::move(original) };
        ASSERT_EQ(InstanceCounter::liveInstances, 2);
    }

    EXPECT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(VectorMemoryTest, MoveAssignmentOperator)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::Vector<InstanceCounter> v1 { {}, {} };
        cads::Vector<InstanceCounter> v2 { {}, {}, {} };
        ASSERT_EQ(InstanceCounter::liveInstances, 5);

        v1 = std::move(v2);

        ASSERT_EQ(v1.size(), 3);
        ASSERT_EQ(InstanceCounter::liveInstances, 3);
    }

    EXPECT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(VectorMemoryTest, PushBackLvalueAndRvalue)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::Vector<InstanceCounter> v;
        const InstanceCounter object;

        ASSERT_EQ(InstanceCounter::liveInstances, 1);

        v.pushBack(object);
        ASSERT_EQ(InstanceCounter::liveInstances, 2);

        v.pushBack(InstanceCounter{});
        ASSERT_EQ(InstanceCounter::liveInstances, 3);
    }

    ASSERT_EQ(InstanceCounter::liveInstances, 0);
}

TEST(VectorMemoryTest, EraseDestructors)
{
    ASSERT_EQ(InstanceCounter::liveInstances, 0);

    {
        cads::Vector<InstanceCounter> vec(5);

        vec.erase(vec.begin() + 1, vec.begin() + 3);
        ASSERT_EQ(InstanceCounter::liveInstances, 3);
        EXPECT_EQ(vec.size(), 3);
        EXPECT_EQ(vec.capacity(), 5);
    }

    ASSERT_EQ(InstanceCounter::liveInstances, 0);
}
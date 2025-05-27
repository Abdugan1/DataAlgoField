#include "pch.h"
#include "../Algorithms/Queue.h"

// Simple fixture for reuse
class QueueTest : public ::testing::Test {
protected:
    Queue<int> q;
};

TEST_F(QueueTest, InitiallyEmpty) {
    EXPECT_TRUE(q.isEmpty());
    EXPECT_EQ(q.size(), 0);
}

TEST_F(QueueTest, PushIncreasesSize) {
    q.push(1);
    q.push(2);
    EXPECT_EQ(q.size(), 2);
}

TEST_F(QueueTest, FrontReturnsCorrectElement) {
    q.push(42);
    EXPECT_EQ(q.front(), 42);
}

TEST_F(QueueTest, PopRemovesFront) {
    q.push(1);
    q.push(2);
    q.pop();
    EXPECT_EQ(q.front(), 2);
    EXPECT_EQ(q.size(), 1);
}

TEST_F(QueueTest, PushRvalue) {
    int x = 99;
    q.push(std::move(x));
    EXPECT_EQ(q.front(), 99);
}

TEST_F(QueueTest, CopyConstructor) {
    q.push(1);
    q.push(2);
    Queue<int> copy(q);
    EXPECT_EQ(copy.size(), 2);
    EXPECT_EQ(copy.front(), 1);
}

TEST_F(QueueTest, MoveConstructor) {
    q.push(7);
    Queue<int> moved(std::move(q));
    EXPECT_EQ(moved.front(), 7);
    EXPECT_EQ(moved.size(), 1);
}

TEST_F(QueueTest, CopyAssignment) {
    Queue<int> other;
    other.push(3);
    q = other;
    EXPECT_EQ(q.front(), 3);
}

TEST_F(QueueTest, MoveAssignment) {
    Queue<int> other;
    other.push(4);
    q = std::move(other);
    EXPECT_EQ(q.front(), 4);
}

TEST_F(QueueTest, ChainedOperations) {
    q.push(10);
    q.push(20);
    q.pop();
    q.push(30);
    EXPECT_EQ(q.front(), 20);
    EXPECT_EQ(q.size(), 2);
}

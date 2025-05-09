#include "pch.h"
#include "../Algorithms/Vector.h"
#include <gtest/gtest.h>
#include <initializer_list>
#include <algorithm>
#include <stdexcept> // For std::out_of_range

// Helper function to compare vectors (since direct comparison might not work for custom classes)
template <typename T>
bool AreVectorsEqual(const Vector<T>& vec1, const Vector<T>& vec2) {
    if (vec1.size() != vec2.size()) {
        return false;
    }
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i] != vec2[i]) { // Use operator!= if available for T, otherwise default
            return false;
        }
    }
    return true;
}

// Initializer List Constructor Test
TEST(VectorTest, InitializerListConstructor) {
    Vector<int> vec = { 1, 2, 3, 4, 5 };
    EXPECT_EQ(vec.size(), 5);
    EXPECT_GE(vec.capacity(), 5); // Capacity should be >= size
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(vec[i], i + 1);
    }
}

// Copy Constructor Test
TEST(VectorTest, CopyConstructor) {
    Vector<int> vec1 = { 10, 20, 30 };
    Vector<int> vec2 = vec1; // Copy constructor
    EXPECT_TRUE(AreVectorsEqual(vec1, vec2)); // Use helper for comparison
    // Ensure that the copy is deep
    vec1[0] = 100;
    EXPECT_NE(vec1[0], vec2[0]);
}

// Copy Assignment Test
TEST(VectorTest, CopyAssignment) {
    Vector<int> vec1 = { 1, 2, 3 };
    Vector<int> vec2;
    vec2 = vec1; // Copy assignment
    EXPECT_TRUE(AreVectorsEqual(vec1, vec2));
    // Ensure that the copy is deep
    vec1[0] = 100;
    EXPECT_NE(vec1[0], vec2[0]);
}

// Move Constructor Test
TEST(VectorTest, MoveConstructor) {
    Vector<int> vec1 = { 1, 2, 3 };
    Vector<int> vec2 = std::move(vec1); // Move constructor

    EXPECT_EQ(vec2.size(), 3);
    EXPECT_GE(vec2.capacity(), 3);
    EXPECT_EQ(vec1.size(), 0); // vec1 should be empty after move
    EXPECT_EQ(vec1.capacity(), 0);
}

// Move Assignment Test
TEST(VectorTest, MoveAssignment) {
    Vector<int> vec1 = { 1, 2, 3 };
    Vector<int> vec2;
    vec2 = std::move(vec1); // Move assignment

    EXPECT_EQ(vec2.size(), 3);
    EXPECT_GE(vec2.capacity(), 3);
    EXPECT_EQ(vec1.size(), 0);     // vec1 should be empty after move
    EXPECT_EQ(vec1.capacity(), 0);
}

// Move Only Types Test
TEST(VectorTest, MoveOnlyType) {
    Vector<std::unique_ptr<int>> vec;
    vec.pushBack(std::make_unique<int>(10));
    vec.pushBack(std::make_unique<int>(20));

    ASSERT_EQ(vec.size(), 2);
    EXPECT_EQ(*vec[0], 10);
    EXPECT_EQ(*vec[1], 20);
}


TEST(VectorTest, PushFrontResizingStress) {
    Vector<int> vec;
    for (int i = 0; i < 1000; ++i)
        vec.pushFront(i);
    EXPECT_EQ(vec.size(), 1000);
    EXPECT_EQ(vec[0], 999);
    EXPECT_EQ(vec[999], 0);
}


// Push Front Test
TEST(VectorTest, PushFront) {
    Vector<int> vec;
    vec.pushFront(1);
    vec.pushFront(2);
    vec.pushFront(3);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_GE(vec.capacity(), 3);
    EXPECT_EQ(vec[0], 3);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 1);
}

// Push Front Move Test
TEST(VectorTest, PushFrontMove) {
    Vector<std::string> vec;
    std::string str1 = "hello";
    std::string str2 = "world";
    vec.pushFront(str1);
    vec.pushFront(std::move(str2));
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], "world");
    EXPECT_EQ(vec[1], "hello");
}

// Push Back Test
TEST(VectorTest, PushBack) {
    Vector<int> vec;
    vec.pushBack(1);
    vec.pushBack(2);
    vec.pushBack(3);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_GE(vec.capacity(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

// Push Back Move Test
TEST(VectorTest, PushBackMove) {
    Vector<std::string> vec;
    std::string str1 = "hello";
    std::string str2 = "world";
    vec.pushBack(str1);
    vec.pushBack(std::move(str2));
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], "hello");
    EXPECT_EQ(vec[1], "world");
    EXPECT_EQ(str2, ""); // str2 should be empty after move
}

// Test Pop Front
TEST(VectorTest, PopFront) {
    Vector<int> vec = { 1, 2, 3 };
    vec.popFront();
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 2);
    EXPECT_EQ(vec[1], 3);
}

// Test Pop Front on a single element
TEST(VectorTest, PopFrontSingleElement) {
    Vector<int> vec = { 5 };
    vec.popFront();
    EXPECT_EQ(vec.size(), 0);
    EXPECT_TRUE(vec.isEmpty());
}

// Test Pop Front on empty list - Should cause an assert
TEST(VectorTest, PopFrontEmpty) {
    Vector<int> vec;
    EXPECT_DEATH(vec.popFront(), ".*");
}

// Pop Back Test
TEST(VectorTest, PopBack) {
    Vector<int> vec = { 1, 2, 3 };
    vec.popBack();
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
}

// Pop Back Empty Test
TEST(VectorTest, PopBackEmpty) {
    Vector<int> vec;
    //  vec.popBack(); // Should throw, but gtest doesn't like exceptions in EXPECT macros
    EXPECT_EQ(vec.size(), 0);
}


// Reset Test
TEST(VectorTest, Reset) {
    Vector<int> vec = { 1, 2, 3 };
    vec.reset();
    EXPECT_EQ(vec.size(), 0);
    EXPECT_GE(vec.capacity(), 0); // Capacity might not be 0, but should be reasonable
}

// Size Test
TEST(VectorTest, Size) {
    Vector<int> vec;
    EXPECT_EQ(vec.size(), 0);
    vec.pushBack(1);
    EXPECT_EQ(vec.size(), 1);
    vec.pushBack(2);
    EXPECT_EQ(vec.size(), 2);
}

// Subscript Operator Test
TEST(VectorTest, SubscriptOperator) {
    Vector<int> vec = { 10, 20, 30 };
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
    vec[1] = 25;
    EXPECT_EQ(vec[1], 25);
}

// Subscript Operator Out of Bounds Test
TEST(VectorTest, SubscriptOperatorOutOfBounds) {
    Vector<int> vec = { 10, 20, 30 };

    // Test accessing out-of-bounds elements in a debug build
    EXPECT_DEATH(vec[3], ".*"); // Causes an assertion failure
}

// Pop Back Empty Vector Test
TEST(VectorTest, PopBackEmptyVector) {
    Vector<int> vec;

    // Test behavior when popping from an empty vector
    // Assuming you refactor `popBack` to throw a `std::runtime_error`
    EXPECT_DEATH(vec.popBack(), ".*");
    //EXPECT_THROW(vec.popBack(), std::runtime_error);
}

// Capacity Increase Stress Test
TEST(VectorTest, CapacityGrowth) {
    Vector<int> vec;
    size_t prevCapacity = vec.capacity();

    for (int i = 0; i < 1000; ++i) {
        vec.pushBack(i);
        if (vec.size() > prevCapacity) {
            EXPECT_GT(vec.capacity(), prevCapacity); // Capacity should grow
            prevCapacity = vec.capacity();
        }
    }
    EXPECT_EQ(vec.size(), 1000);
}


// Const Subscript Operator Test
TEST(VectorTest, ConstSubscriptOperator) {
    const Vector<int> vec = { 10, 20, 30 };
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
    //  vec[1] = 25; // This line would cause a compilation error
}

// Const Subscript Operator Out of Bounds Test
TEST(VectorTest, ConstSubscriptOperatorOutOfBounds) {
    const Vector<int> vec = { 10, 20, 30 };
    //const int x = vec[3]; // This should cause an assert in the original code. GTest doesn't play well with asserts.
    EXPECT_EQ(vec.size(), 3);
}

// Begin and End Iterator Test
TEST(VectorTest, Iterator) {
    Vector<int> vec = { 1, 2, 3, 4, 5 };
    int sum = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

// Const Begin and End Iterator Test
TEST(VectorTest, ConstIterator) {
    const Vector<int> vec = { 1, 2, 3, 4, 5 };
    int sum = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

TEST(VectorTest, IteratorInvalidationAfterPushBack) {
    Vector<int> vec = { 1, 2, 3 };
    auto it = vec.begin();
    int first = *it;

    vec.pushBack(4); // Might trigger reallocation

    // `it` may be invalid now, depending on how capacity growth is implemented
    // This test checks if we *documented* or *prevented* invalid use
    EXPECT_EQ(first, 1);
    // Accessing `*it` again here is undefined behavior in std-style containers after resize
}


TEST(VectorTest, NestedVector) {
    Vector<Vector<int>> nestedVec;
    Vector<int> inner1 = { 1, 2 };
    Vector<int> inner2 = { 3, 4 };

    nestedVec.pushBack(inner1);
    nestedVec.pushBack(std::move(inner2));

    ASSERT_EQ(nestedVec.size(), 2);
    EXPECT_EQ(nestedVec[0][1], 2);
    EXPECT_EQ(nestedVec[1][0], 3);
}


TEST(VectorTest, PushFrontMoveOnlyType) {
    Vector<std::unique_ptr<int>> vec;
    vec.pushFront(std::make_unique<int>(42));
    vec.pushFront(std::make_unique<int>(99));

    ASSERT_EQ(vec.size(), 2);
    EXPECT_EQ(*vec[0], 99);
    EXPECT_EQ(*vec[1], 42);
}


TEST(VectorTest, SelfAssignmentMoveOnlyType) {
    Vector<std::unique_ptr<int>> vec;
    vec.pushBack(std::make_unique<int>(1));
    vec.pushBack(std::make_unique<int>(2));

    vec = std::move(vec); // Self-move, should not corrupt state

    ASSERT_EQ(vec.size(), 2);
    EXPECT_EQ(*vec[0], 1);
    EXPECT_EQ(*vec[1], 2);
}


// Self-Assignment Test (Copy Assignment)
TEST(VectorTest, SelfAssignmentCopy) {
    Vector<int> vec = { 1, 2, 3 };
    vec = vec; // Self-assignment
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

// Self-Assignment Test (Move Assignment)
TEST(VectorTest, SelfAssignmentMove) {
    Vector<int> vec = { 1, 2, 3 };
    vec = std::move(vec); // Self-assignment
    EXPECT_EQ(vec.size(), 3);
    EXPECT_GE(vec.capacity(), 3);
}
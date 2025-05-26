#include "pch.h"
#include "../Algorithms/SingleLinkedList.h"
#include <vector>
#include <numeric> // For std::iota

TEST(SingleLinkedListTest, DefaultConstructor) {
    SingleLinkedList<int> list;
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(list.size(), 0);
}

TEST(SingleLinkedListTest, InitializerListConstructor) {
    SingleLinkedList<int> list{ 1, 2, 3, 4, 5 };
    ASSERT_FALSE(list.isEmpty());
    ASSERT_EQ(list.size(), 5);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 5);

    std::vector<int> expected = { 1, 2, 3, 4, 5 };
    size_t i = 0;
    for (const auto& val : list) {
        ASSERT_EQ(val, expected[i++]);
    }
}

TEST(SingleLinkedListTest, CopyConstructor) {
    SingleLinkedList<int> original{ 10, 20, 30 };
    SingleLinkedList<int> copy = original;
    ASSERT_FALSE(copy.isEmpty());
    ASSERT_EQ(copy.size(), 3);
    ASSERT_EQ(copy.front(), 10);
    ASSERT_EQ(copy.back(), 30);

    // Modify the original list to ensure the copy is independent
    original.pushFront(5);
    ASSERT_EQ(original.size(), 4);
    ASSERT_EQ(copy.size(), 3);
    ASSERT_EQ(copy.front(), 10);
}

TEST(SingleLinkedListTest, MoveConstructor) {
    SingleLinkedList<int> original{ 100, 200 };
    SingleLinkedList<int> moved = std::move(original);
    ASSERT_TRUE(original.isEmpty());
    ASSERT_EQ(original.size(), 0);
    ASSERT_FALSE(moved.isEmpty());
    ASSERT_EQ(moved.size(), 2);
    ASSERT_EQ(moved.front(), 100);
    ASSERT_EQ(moved.back(), 200);
}

TEST(SingleLinkedListTest, Destructor) {
    // This test mainly checks for memory leaks, which is harder to automate in a unit test.
    // We can create and destroy a list and rely on memory sanitizers (like AddressSanitizer)
    // during testing to detect issues.
    {
        SingleLinkedList<double> list{ 1.1, 2.2, 3.3 };
    }
    // If there were no memory leaks, the program should continue without errors.
}

TEST(SingleLinkedListTest, PushFront) {
    SingleLinkedList<char> list;
    list.pushFront('c');
    ASSERT_FALSE(list.isEmpty());
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list.front(), 'c');
    ASSERT_EQ(list.back(), 'c');

    list.pushFront('b');
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.front(), 'b');
    ASSERT_EQ(list.back(), 'c');

    list.pushFront('a');
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list.front(), 'a');
    ASSERT_EQ(list.back(), 'c');
}

TEST(SingleLinkedListTest, PushBack) {
    SingleLinkedList<int> list;
    list.pushBack(1);
    ASSERT_FALSE(list.isEmpty());
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 1);

    list.pushBack(2);
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 2);

    list.pushBack(3);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 3);
}

TEST(SingleLinkedListTest, PopFront) {
    SingleLinkedList<int> list{ 1, 2, 3 };
    list.popFront();
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.front(), 2);
    ASSERT_EQ(list.back(), 3);

    list.popFront();
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list.front(), 3);
    ASSERT_EQ(list.back(), 3);

    list.popFront();
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(list.size(), 0);
}

TEST(SingleLinkedListTest, PopFrontOnEmptyList) {
    SingleLinkedList<int> list;
    ASSERT_DEATH(list.popFront(), ".*");
}

TEST(SingleLinkedListTest, PopBack) {
    SingleLinkedList<int> list{ 1, 2, 3 };
    list.popBack();
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 2);

    list.popBack();
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 1);

    list.popBack();
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(list.size(), 0);
}

TEST(SingleLinkedListTest, PopBackOnEmptyList) {
    SingleLinkedList<int> list;
    ASSERT_DEATH(list.popBack(), ".*");
}

TEST(SingleLinkedListTest, Clear) {
    SingleLinkedList<std::string> list{ "hello", "world" };
    list.clear();
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(list.size(), 0);
    // Ensure head and tail are also null
    ASSERT_EQ(list.end(), list.begin());
    ASSERT_EQ(list.end(), list.end());
}

TEST(SingleLinkedListTest, SizeAndIsEmpty) {
    SingleLinkedList<int> list1;
    ASSERT_TRUE(list1.isEmpty());
    ASSERT_EQ(list1.size(), 0);

    SingleLinkedList<int> list2{ 1, 2 };
    ASSERT_FALSE(list2.isEmpty());
    ASSERT_EQ(list2.size(), 2);

    list2.popFront();
    ASSERT_FALSE(list2.isEmpty());
    ASSERT_EQ(list2.size(), 1);

    list2.popFront();
    ASSERT_TRUE(list2.isEmpty());
    ASSERT_EQ(list2.size(), 0);
}

TEST(SingleLinkedListTest, AssignmentOperatorCopy) {
    SingleLinkedList<int> original{ 5, 6, 7 };
    SingleLinkedList<int> assigned;
    assigned = original;

    ASSERT_FALSE(assigned.isEmpty());
    ASSERT_EQ(assigned.size(), 3);
    ASSERT_EQ(assigned.front(), 5);
    ASSERT_EQ(assigned.back(), 7);

    // Modify the original to ensure independence
    original.pushFront(4);
    ASSERT_EQ(original.size(), 4);
    ASSERT_EQ(assigned.size(), 3);
    ASSERT_EQ(assigned.front(), 5);

    // Self-assignment should be handled correctly
    assigned = assigned;
    ASSERT_FALSE(assigned.isEmpty());
    ASSERT_EQ(assigned.size(), 3);
    ASSERT_EQ(assigned.front(), 5);
    ASSERT_EQ(assigned.back(), 7);
}

TEST(SingleLinkedListTest, AssignmentOperatorMove) {
    SingleLinkedList<int> original{ 8, 9 };
    SingleLinkedList<int> assigned;
    assigned = std::move(original);

    ASSERT_TRUE(original.isEmpty());
    ASSERT_EQ(original.size(), 0);
    ASSERT_FALSE(assigned.isEmpty());
    ASSERT_EQ(assigned.size(), 2);
    ASSERT_EQ(assigned.front(), 8);
    ASSERT_EQ(assigned.back(), 9);

    // Self-move should be handled correctly (though less meaningful)
    assigned = std::move(assigned);
    ASSERT_FALSE(assigned.isEmpty());
    ASSERT_EQ(assigned.size(), 2);
    ASSERT_EQ(assigned.front(), 8);
    ASSERT_EQ(assigned.back(), 9);
}

TEST(SingleLinkedListTest, FrontAndBack) {
    SingleLinkedList<double> list{ 3.14, 2.718, 1.618 };
    ASSERT_EQ(list.front(), 3.14);
    ASSERT_EQ(list.back(), 1.618);

    list.front() = 1.0;
    ASSERT_EQ(list.front(), 1.0);

    list.back() = 2.0;
    ASSERT_EQ(list.back(), 2.0);

    const SingleLinkedList<double> constList = { 6.0, 7.0 };
    ASSERT_EQ(constList.front(), 6.0);
    ASSERT_EQ(constList.back(), 7.0);
}

TEST(SingleLinkedListTest, FrontAndBackOnEmptyList) {
    SingleLinkedList<int> list;
    ASSERT_DEATH(list.front(), ".*");
    ASSERT_DEATH(list.back(), ".*");

    const SingleLinkedList<int> constList;
    ASSERT_DEATH(constList.front(), ".*");
    ASSERT_DEATH(constList.back(), ".*");
}

TEST(SingleLinkedListTest, IteratorBeginAndEnd) {
    SingleLinkedList<int> list{ 5, 10, 15 };
    std::vector<int> result;
    for (auto it = list.begin(); it != list.end(); ++it) {
        result.push_back(*it);
    }
    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result[0], 5);
    ASSERT_EQ(result[1], 10);
    ASSERT_EQ(result[2], 15);
}

TEST(SingleLinkedListTest, ConstIteratorBeginAndEnd) {
    const SingleLinkedList<int> list{ 2, 4, 6 };
    std::vector<int> result;
    for (auto it = list.cbegin(); it != list.cend(); ++it) {
        result.push_back(*it);
    }
    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result[0], 2);
    ASSERT_EQ(result[1], 4);
    ASSERT_EQ(result[2], 6);

    std::vector<int> result2;
    for (const auto& val : list) {
        result2.push_back(val);
    }
    ASSERT_EQ(result2.size(), 3);
    ASSERT_EQ(result2[0], 2);
    ASSERT_EQ(result2[1], 4);
    ASSERT_EQ(result2[2], 6);
}

TEST(SingleLinkedListTest, IteratorIncrement) {
    SingleLinkedList<int> list{ 1, 3, 5 };
    auto it = list.begin();
    ASSERT_EQ(*it, 1);
    ++it;
    ASSERT_EQ(*it, 3);
    auto it2 = it++;
    ASSERT_EQ(*it, 5);
    ASSERT_EQ(*it2, 3);
    ++it; // Now it should be equal to end()
    ASSERT_EQ(it, list.end());
}

TEST(SingleLinkedListTest, ConstIteratorIncrement) {
    const SingleLinkedList<int> list{ 2, 4, 6 };
    auto it = list.cbegin();
    ASSERT_EQ(*it, 2);
    ++it;
    ASSERT_EQ(*it, 4);
    auto it2 = it++;
    ASSERT_EQ(*it, 6);
    ASSERT_EQ(*it2, 4);
    ++it; // Now it should be equal to end()
    ASSERT_EQ(it, list.cend());
}

TEST(SingleLinkedListTest, IteratorDereference) {
    SingleLinkedList<std::string> list{ "apple", "banana" };
    auto it = list.begin();
    ASSERT_EQ(*it, "apple");
    *it = "apricot";
    ASSERT_EQ(list.front(), "apricot");

    const SingleLinkedList<std::string> constList{ "orange", "grape" };
    auto constIt = constList.cbegin();
    ASSERT_EQ(*constIt, "orange");
    // Attempting to modify through const iterator should result in a compile error.
    // (We can't directly test for compile errors in a unit test)
}

TEST(SingleLinkedListTest, IteratorEqualityAndInequality) {
    SingleLinkedList<int> list{ 1 };
    auto beginIt = list.begin();
    auto endIt = list.end();
    ASSERT_EQ(beginIt, beginIt);
    ASSERT_NE(beginIt, endIt);
    ASSERT_EQ(endIt, endIt);
    ASSERT_NE(endIt, beginIt);

    ++beginIt;
    ASSERT_EQ(beginIt, endIt);
}

TEST(SingleLinkedListTest, RemoveFromBeginning) {
    SingleLinkedList<int> list{ 1, 2, 3 };
    auto it = list.begin();
    list.remove(it);
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.front(), 2);
    ASSERT_EQ(list.back(), 3);
    std::vector<int> expected = { 2, 3 };
    size_t i = 0;
    for (const auto& val : list) {
        ASSERT_EQ(val, expected[i++]);
    }
}

TEST(SingleLinkedListTest, RemoveFromMiddle) {
    SingleLinkedList<int> list{ 10, 20, 30, 40 };
    auto it = list.begin();
    ++it; // Points to 20
    list.remove(it);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list.front(), 10);
    ASSERT_EQ(list.back(), 40);
    std::vector<int> expected = { 10, 30, 40 };
    size_t i = 0;
    for (const auto& val : list) {
        ASSERT_EQ(val, expected[i++]);
    }
}

TEST(SingleLinkedListTest, RemoveFromEnd) {
    SingleLinkedList<int> list{ 100, 200, 300 };
    auto it = list.begin();
    ++it;
    ++it; // Points to 300 (the last element)
    list.remove(it);
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.front(), 100);
    ASSERT_EQ(list.back(), 200);
    std::vector<int> expected = { 100, 200 };
    size_t i = 0;
    for (const auto& val : list) {
        ASSERT_EQ(val, expected[i++]);
    }
}

TEST(SingleLinkedListTest, RemoveOnlyElement) {
    SingleLinkedList<int> list{ 5 };
    auto it = list.begin();
    list.remove(it);
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(list.size(), 0);
    ASSERT_EQ(list.begin(), list.end());
}

TEST(SingleLinkedListTest, RemoveInvalidIterator) {
    SingleLinkedList<int> list1{ 1, 2 };
    SingleLinkedList<int> list2{ 3 };
    auto it = list2.begin();
    ASSERT_DEATH(list1.remove(it), ".*");
}

TEST(SingleLinkedListTest, RemoveEndIterator) {
    SingleLinkedList<int> list{ 1 };
    ASSERT_DEATH(list.remove(list.end()), ".*");
}

TEST(SingleLinkedListTest, RemoveOnEmptyListIterator) {
    SingleLinkedList<int> list;
    ASSERT_DEATH(list.remove(list.begin()), ".*");
}

TEST(SingleLinkedListTest, RangeBasedForLoop) {
    SingleLinkedList<char> list{ 'p', 'q', 'r' };
    std::string result = "";
    for (char c : list) {
        result += c;
    }
    ASSERT_EQ(result, "pqr");

    const SingleLinkedList<char> constList{ 'x', 'y' };
    std::string constResult = "";
    for (const char& c : constList) {
        constResult += c;
    }
    ASSERT_EQ(constResult, "xy");
}

TEST(SingleLinkedListTest, LargeNumberOfElements) {
    SingleLinkedList<int> list;
    const size_t count = 1000;
    for (size_t i = 0; i < count; ++i) {
        list.pushBack(static_cast<int>(i));
    }
    ASSERT_EQ(list.size(), count);
    ASSERT_EQ(list.front(), 0);
    ASSERT_EQ(list.back(), 999);

    size_t i = 0;
    for (const auto& val : list) {
        ASSERT_EQ(val, static_cast<int>(i++));
    }

    for (size_t i = 0; i < count / 2; ++i) {
        list.popFront();
    }
    ASSERT_EQ(list.size(), count / 2);
    ASSERT_EQ(list.front(), 500);
    ASSERT_EQ(list.back(), 999);

    list.clear();
    ASSERT_TRUE(list.isEmpty());
    ASSERT_EQ(list.size(), 0);
}
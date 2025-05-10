#include "pch.h"
//#include "../Algorithms/DoubleLinkedList.h"
//#include <vector>
//#include <algorithm> // For std::equal
//#include <numeric>
//
//TEST(DoubleLinkedListTest, DefaultConstructor)
//{
//    DoubleLinkedList<int> list;
//    ASSERT_TRUE(list.isEmpty());
//    ASSERT_EQ(list.size(), 0);
//}
//
//TEST(DoubleLinkedListTest, InitializerListConstructor) {
//    DoubleLinkedList<int> list{ 1, 2, 3, 4, 5 };
//    ASSERT_FALSE(list.isEmpty());
//    ASSERT_EQ(list.size(), 5);
//    ASSERT_EQ(list.front(), 1);
//    ASSERT_EQ(list.back(), 5);
//
//    std::vector<int> expected = { 1, 2, 3, 4, 5 };
//    size_t i = 0;
//    for (const auto& val : list) {
//        ASSERT_EQ(val, expected[i++]);
//    }
//}
//
//TEST(DoubleLinkedListTest, CopyConstructor) {
//    DoubleLinkedList<int> original{ 10, 20, 30 };
//    DoubleLinkedList<int> copy = original;
//    ASSERT_FALSE(copy.isEmpty());
//    ASSERT_EQ(copy.size(), 3);
//    ASSERT_EQ(copy.front(), 10);
//    ASSERT_EQ(copy.back(), 30);
//
//    // Modify the original list to ensure the copy is independent
//    original.pushFront(5);
//    ASSERT_EQ(original.size(), 4);
//    ASSERT_EQ(copy.size(), 3);
//    ASSERT_EQ(copy.front(), 10);
//}
//
//TEST(DoubleLinkedListTest, MoveConstructor) {
//    DoubleLinkedList<int> original{ 100, 200 };
//    DoubleLinkedList<int> moved = std::move(original);
//    ASSERT_TRUE(original.isEmpty());
//    ASSERT_EQ(original.size(), 0);
//    ASSERT_FALSE(moved.isEmpty());
//    ASSERT_EQ(moved.size(), 2);
//    ASSERT_EQ(moved.front(), 100);
//    ASSERT_EQ(moved.back(), 200);
//}
//
//TEST(DoubleLinkedListTest, Destructor) {
//    // Primarily for memory leak detection (using tools like AddressSanitizer)
//    {
//        DoubleLinkedList<double> list{ 1.1, 2.2, 3.3 };
//    }
//}
//
//TEST(DoubleLinkedListTest, PushFront) {
//    DoubleLinkedList<char> list;
//    list.pushFront('c');
//    ASSERT_FALSE(list.isEmpty());
//    ASSERT_EQ(list.size(), 1);
//    ASSERT_EQ(list.front(), 'c');
//    ASSERT_EQ(list.back(), 'c');
//
//    list.pushFront('b');
//    ASSERT_EQ(list.size(), 2);
//    ASSERT_EQ(list.front(), 'b');
//    ASSERT_EQ(list.back(), 'c');
//
//    list.pushFront('a');
//    ASSERT_EQ(list.size(), 3);
//    ASSERT_EQ(list.front(), 'a');
//    ASSERT_EQ(list.back(), 'c');
//}
//
//TEST(DoubleLinkedListTest, PushBack) {
//    DoubleLinkedList<int> list;
//    list.pushBack(1);
//    ASSERT_FALSE(list.isEmpty());
//    ASSERT_EQ(list.size(), 1);
//    ASSERT_EQ(list.front(), 1);
//    ASSERT_EQ(list.back(), 1);
//
//    list.pushBack(2);
//    ASSERT_EQ(list.size(), 2);
//    ASSERT_EQ(list.front(), 1);
//    ASSERT_EQ(list.back(), 2);
//
//    list.pushBack(3);
//    ASSERT_EQ(list.size(), 3);
//    ASSERT_EQ(list.front(), 1);
//    ASSERT_EQ(list.back(), 3);
//}
//
//TEST(DoubleLinkedListTest, PopFront) {
//    DoubleLinkedList<int> list{ 1, 2, 3 };
//    list.popFront();
//    ASSERT_EQ(list.size(), 2);
//    ASSERT_EQ(list.front(), 2);
//    ASSERT_EQ(list.back(), 3);
//
//    list.popFront();
//    ASSERT_EQ(list.size(), 1);
//    ASSERT_EQ(list.front(), 3);
//    ASSERT_EQ(list.back(), 3);
//
//    list.popFront();
//    ASSERT_TRUE(list.isEmpty());
//    ASSERT_EQ(list.size(), 0);
//}
//
//TEST(DoubleLinkedListTest, PopFrontOnEmptyList) {
//    DoubleLinkedList<int> list;
//    ASSERT_DEATH(list.popFront(), ".*");
//}
//
//TEST(DoubleLinkedListTest, PopBack) {
//    DoubleLinkedList<int> list{ 1, 2, 3 };
//    list.popBack();
//    ASSERT_EQ(list.size(), 2);
//    ASSERT_EQ(list.front(), 1);
//    ASSERT_EQ(list.back(), 2);
//
//    list.popBack();
//    ASSERT_EQ(list.size(), 1);
//    ASSERT_EQ(list.front(), 1);
//    ASSERT_EQ(list.back(), 1);
//
//    list.popBack();
//    ASSERT_TRUE(list.isEmpty());
//    ASSERT_EQ(list.size(), 0);
//}
//
//TEST(DoubleLinkedListTest, PopBackOnEmptyList) {
//    DoubleLinkedList<int> list;
//    ASSERT_DEATH(list.popBack(), ".*");
//}
//
//TEST(DoubleLinkedListTest, Clear) {
//    DoubleLinkedList<std::string> list{ "hello", "world" };
//    list.clear();
//    ASSERT_TRUE(list.isEmpty());
//    ASSERT_EQ(list.size(), 0);
//    ASSERT_EQ(list.begin(), list.end());
//}
//
//TEST(DoubleLinkedListTest, SizeAndIsEmpty) {
//    DoubleLinkedList<int> list1;
//    ASSERT_TRUE(list1.isEmpty());
//    ASSERT_EQ(list1.size(), 0);
//
//    DoubleLinkedList<int> list2{ 1, 2 };
//    ASSERT_FALSE(list2.isEmpty());
//    ASSERT_EQ(list2.size(), 2);
//
//    list2.popFront();
//    ASSERT_FALSE(list2.isEmpty());
//    ASSERT_EQ(list2.size(), 1);
//
//    list2.popFront();
//    ASSERT_TRUE(list2.isEmpty());
//    ASSERT_EQ(list2.size(), 0);
//}
//
//TEST(DoubleLinkedListTest, AssignmentOperatorCopy) {
//    DoubleLinkedList<int> original{ 5, 6, 7 };
//    DoubleLinkedList<int> assigned;
//    assigned = original;
//
//    ASSERT_FALSE(assigned.isEmpty());
//    ASSERT_EQ(assigned.size(), 3);
//    ASSERT_EQ(assigned.front(), 5);
//    ASSERT_EQ(assigned.back(), 7);
//
//    // Modify the original to ensure independence
//    original.pushFront(4);
//    ASSERT_EQ(original.size(), 4);
//    ASSERT_EQ(assigned.size(), 3);
//    ASSERT_EQ(assigned.front(), 5);
//
//    // Self-assignment
//    assigned = assigned;
//    ASSERT_FALSE(assigned.isEmpty());
//    ASSERT_EQ(assigned.size(), 3);
//    ASSERT_EQ(assigned.front(), 5);
//    ASSERT_EQ(assigned.back(), 7);
//}
//
//TEST(DoubleLinkedListTest, AssignmentOperatorMove) {
//    DoubleLinkedList<int> original{ 8, 9 };
//    DoubleLinkedList<int> assigned;
//    assigned = std::move(original);
//
//    ASSERT_TRUE(original.isEmpty());
//    ASSERT_EQ(original.size(), 0);
//    ASSERT_FALSE(assigned.isEmpty());
//    ASSERT_EQ(assigned.size(), 2);
//    ASSERT_EQ(assigned.front(), 8);
//    ASSERT_EQ(assigned.back(), 9);
//
//    // Self-move
//    assigned = std::move(assigned);
//    ASSERT_FALSE(assigned.isEmpty());
//    ASSERT_EQ(assigned.size(), 2);
//    ASSERT_EQ(assigned.front(), 8);
//    ASSERT_EQ(assigned.back(), 9);
//}
//
//TEST(DoubleLinkedListTest, FrontAndBack) {
//    DoubleLinkedList<double> list{ 3.14, 2.718, 1.618 };
//    ASSERT_EQ(list.front(), 3.14);
//    ASSERT_EQ(list.back(), 1.618);
//
//    list.front() = 1.0;
//    ASSERT_EQ(list.front(), 1.0);
//
//    list.back() = 2.0;
//    ASSERT_EQ(list.back(), 2.0);
//
//    const DoubleLinkedList<double> constList = { 6.0, 7.0 };
//    ASSERT_EQ(constList.front(), 6.0);
//    ASSERT_EQ(constList.back(), 7.0);
//}
//
//TEST(DoubleLinkedListTest, FrontAndBackOnEmptyList) {
//    DoubleLinkedList<int> list;
//    ASSERT_DEATH(list.front(), ".*");
//    ASSERT_DEATH(list.back(), ".*");
//
//    const DoubleLinkedList<int> constList;
//    ASSERT_DEATH(constList.front(), ".*");
//    ASSERT_DEATH(constList.back(), ".*");
//}
//
//TEST(DoubleLinkedListTest, IteratorBeginAndEnd) {
//    DoubleLinkedList<int> list{ 5, 10, 15 };
//    std::vector<int> result;
//    for (auto it = list.begin(); it != list.end(); ++it) {
//        result.push_back(*it);
//    }
//    ASSERT_EQ(result.size(), 3);
//    ASSERT_EQ(result[0], 5);
//    ASSERT_EQ(result[1], 10);
//    ASSERT_EQ(result[2], 15);
//}
//
//TEST(DoubleLinkedListTest, ConstIteratorBeginAndEnd) {
//    const DoubleLinkedList<int> list{ 2, 4, 6 };
//    std::vector<int> result;
//    for (auto it = list.cbegin(); it != list.cend(); ++it) {
//        result.push_back(*it);
//    }
//    ASSERT_EQ(result.size(), 3);
//    ASSERT_EQ(result[0], 2);
//    ASSERT_EQ(result[1], 4);
//    ASSERT_EQ(result[2], 6);
//
//    std::vector<int> result2;
//    for (const auto& val : list) {
//        result2.push_back(val);
//    }
//    ASSERT_EQ(result2.size(), 3);
//    ASSERT_EQ(result2[0], 2);
//    ASSERT_EQ(result2[1], 4);
//    ASSERT_EQ(result2[2], 6);
//}
//
//TEST(DoubleLinkedListTest, IteratorIncrement) {
//    DoubleLinkedList<int> list{ 1, 3, 5 };
//    auto it = list.begin();
//    ASSERT_EQ(*it, 1);
//    ++it;
//    ASSERT_EQ(*it, 3);
//    auto it2 = it++;
//    ASSERT_EQ(*it, 5);
//    ASSERT_EQ(*it2, 3);
//    ++it; // Now it should be equal to end()
//    ASSERT_EQ(it, list.end());
//}
//
//TEST(DoubleLinkedListTest, ConstIteratorIncrement) {
//    const DoubleLinkedList<int> list{ 2, 4, 6 };
//    auto it = list.cbegin();
//    ASSERT_EQ(*it, 2);
//    ++it;
//    ASSERT_EQ(*it, 4);
//    auto it2 = it++;
//    ASSERT_EQ(*it, 6);
//    ASSERT_EQ(*it2, 4);
//    ++it; // Now it should be equal to end()
//    ASSERT_EQ(it, list.cend());
//}
//
//TEST(DoubleLinkedListTest, IteratorDereference) {
//    DoubleLinkedList<std::string> list{ "apple", "banana" };
//    auto it = list.begin();
//    ASSERT_EQ(*it, "apple");
//    *it = "apricot";
//    ASSERT_EQ(list.front(), "apricot");
//
//    const DoubleLinkedList<std::string> constList{ "orange", "grape" };
//    auto constIt = constList.cbegin();
//    ASSERT_EQ(*constIt, "orange");
//    // Attempting to modify through const iterator should result in a compile error.
//}
//
//TEST(DoubleLinkedListTest, IteratorEqualityAndInequality) {
//    DoubleLinkedList<int> list{ 1 };
//    auto beginIt = list.begin();
//    auto endIt = list.end();
//    ASSERT_EQ(beginIt, beginIt);
//    ASSERT_NE(beginIt, endIt);
//    ASSERT_EQ(endIt, endIt);
//    ASSERT_NE(endIt, beginIt);
//
//    ++beginIt;
//    ASSERT_EQ(beginIt, endIt);
//}
//
//TEST(DoubleLinkedListTest, RemoveFromBeginning) {
//    DoubleLinkedList<int> list{ 1, 2, 3 };
//    auto it = list.begin();
//    list.remove(it);
//    ASSERT_EQ(list.size(), 2);
//    ASSERT_EQ(list.front(), 2);
//    ASSERT_EQ(list.back(), 3);
//    std::vector<int> expected = { 2, 3 };
//    size_t i = 0;
//    for (const auto& val : list) {
//        ASSERT_EQ(val, expected[i++]);
//    }
//}
//
//TEST(DoubleLinkedListTest, RemoveFromMiddle) {
//    DoubleLinkedList<int> list{ 10, 20, 30, 40 };
//    auto it = list.begin();
//    ++it; // Points to 20
//    list.remove(it);
//    ASSERT_EQ(list.size(), 3);
//    ASSERT_EQ(list.front(), 10);
//    ASSERT_EQ(list.back(), 40);
//    std::vector<int> expected = { 10, 30, 40 };
//    size_t i = 0;
//    for (const auto& val : list) {
//        ASSERT_EQ(val, expected[i++]);
//    }
//}
//
//TEST(DoubleLinkedListTest, RemoveFromEnd) {
//    DoubleLinkedList<int> list{ 100, 200, 300 };
//    auto it = list.begin();
//    ++it;
//    ++it; // Points to 300 (the last element)
//    list.remove(it);
//    ASSERT_EQ(list.size(), 2);
//    ASSERT_EQ(list.front(), 100);
//    ASSERT_EQ(list.back(), 200);
//    std::vector<int> expected = { 100, 200 };
//    size_t i = 0;
//    for (const auto& val : list) {
//        ASSERT_EQ(val, expected[i++]);
//    }
//}
//
//TEST(DoubleLinkedListTest, RemoveOnlyElement) {
//    DoubleLinkedList<int> list{ 5 };
//    auto it = list.begin();
//    list.remove(it);
//    ASSERT_TRUE(list.isEmpty());
//    ASSERT_EQ(list.size(), 0);
//    ASSERT_EQ(list.begin(), list.end());
//}
//
//TEST(DoubleLinkedListTest, RemoveInvalidIterator) {
//    DoubleLinkedList<int> list1{ 1, 2 };
//    DoubleLinkedList<int> list2{ 3 };
//    auto it = list2.begin();
//    ASSERT_DEATH(list1.remove(it), ".*");
//}
//
//TEST(DoubleLinkedListTest, RangeBasedForLoop) {
//    DoubleLinkedList<char> list{ 'p', 'q', 'r' };
//    std::string result = "";
//    for (char c : list) {
//        result += c;
//    }
//    ASSERT_EQ(result, "pqr");
//
//    const DoubleLinkedList<char> constList{ 'x', 'y' };
//    std::string constResult = "";
//    for (const char& c : constList) {
//        constResult += c;
//    }
//    ASSERT_EQ(constResult, "xy");
//}
//
////TEST(DoubleLinkedListTest, LargeNumberOfElements) {
////    DoubleLinkedList<int> list;
////    const size_t count = 1000;
////    for (size_t i = 0; i < count; ++i) {
////        list.pushBack(static_cast<int>(i));
////    }
////    ASSERT_EQ(list.size(), count);
////    ASSERT_EQ(list.front(), 0);
////    ASSERT_EQ(list.back(), 999);
////
////    std::vector<int> expected(count);
////    std::iota(expected.begin(), expected.end(), 0);
////    ASSERT_TRUE(std::equal(list.begin(), list.end(), expected.begin(), expected.end()));
////
////    for (size_t i = 0; i < count / 2; ++i) {
////        list.popFront();
////    }
////    ASSERT_EQ(list.size(), count / 2);
////    ASSERT_EQ(list.front(), 500);
////    ASSERT_EQ(list.back(), 999);
////
////    list.clear();
////    ASSERT_TRUE(list.isEmpty());
////    ASSERT_EQ(list.size(), 0);
////}
//
////TEST(DoubleLinkedListTest, GetElementAtIndex) {
////    DoubleLinkedList<char> list{ 'a', 'b', 'c', 'd' };
////    ASSERT_EQ(*list.get(0), 'a');
////    ASSERT_EQ(*list.get(1), 'b');
////    ASSERT_EQ(*list.get(2), 'c');
////    ASSERT_EQ(*list.get(3), 'd');
////    ASSERT_EQ(list.get(4), nullptr); // Out of bounds
////
////    const DoubleLinkedList<char> constList{ 'e', 'f', 'g' };
////    ASSERT_EQ(*constList.get(0), 'e');
////    ASSERT_EQ(*constList.get(1), 'f');
////    ASSERT_EQ(*constList.get(2), 'g');
////    ASSERT_EQ(constList.get(3), nullptr); // Out of bounds
////}
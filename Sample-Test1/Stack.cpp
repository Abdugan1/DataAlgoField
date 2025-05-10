#include "pch.h"
//#include "../Algorithms/Stack.h"
//#include <stdexcept> // For std::underflow_error (if you decide to test exceptions later)
//
//TEST(StackTest, DefaultConstructor) {
//    Stack<int> stack;
//    ASSERT_TRUE(stack.isEmpty());
//    ASSERT_EQ(stack.size(), 0);
//}
//
//TEST(StackTest, InitializerListConstructor) {
//    Stack<int> stack{ 1, 2, 3, 4, 5 };
//    ASSERT_FALSE(stack.isEmpty());
//    ASSERT_EQ(stack.size(), 5);
//    ASSERT_EQ(stack.peek(), 5); // Last element should be on top
//}
//
//TEST(StackTest, CopyConstructor) {
//    Stack<int> original{ 10, 20, 30 };
//    Stack<int> copy = original;
//    ASSERT_FALSE(copy.isEmpty());
//    ASSERT_EQ(copy.size(), 3);
//    ASSERT_EQ(copy.peek(), 30);
//
//    // Modify the original to ensure the copy is independent
//    original.push(40);
//    ASSERT_EQ(original.size(), 4);
//    ASSERT_EQ(original.peek(), 40);
//    ASSERT_EQ(copy.size(), 3);
//    ASSERT_EQ(copy.peek(), 30);
//}
//
//TEST(StackTest, MoveConstructor) {
//    Stack<int> original{ 100, 200 };
//    Stack<int> moved = std::move(original);
//    ASSERT_TRUE(original.isEmpty());
//    ASSERT_EQ(original.size(), 0);
//    ASSERT_FALSE(moved.isEmpty());
//    ASSERT_EQ(moved.size(), 2);
//    ASSERT_EQ(moved.peek(), 200);
//}
//
//TEST(StackTest, Push) {
//    Stack<char> stack;
//    stack.push('a');
//    ASSERT_FALSE(stack.isEmpty());
//    ASSERT_EQ(stack.size(), 1);
//    ASSERT_EQ(stack.peek(), 'a');
//
//    stack.push('b');
//    ASSERT_EQ(stack.size(), 2);
//    ASSERT_EQ(stack.peek(), 'b');
//
//    stack.push('c');
//    ASSERT_EQ(stack.size(), 3);
//    ASSERT_EQ(stack.peek(), 'c');
//}
//
//TEST(StackTest, Pop) {
//    Stack<int> stack{ 1, 2, 3 };
//    stack.pop();
//    ASSERT_EQ(stack.size(), 2);
//    ASSERT_EQ(stack.peek(), 2);
//
//    stack.pop();
//    ASSERT_EQ(stack.size(), 1);
//    ASSERT_EQ(stack.peek(), 1);
//
//    stack.pop();
//    ASSERT_TRUE(stack.isEmpty());
//    ASSERT_EQ(stack.size(), 0);
//}
//
//TEST(StackTest, PopOnEmptyStack) {
//    Stack<int> stack;
//    ASSERT_DEATH(stack.pop(), "isEmpty");
//}
//
//TEST(StackTest, Peek) {
//    Stack<double> stack;
//    stack.push(3.14);
//    ASSERT_EQ(stack.peek(), 3.14);
//    ASSERT_EQ(stack.size(), 1); // Peek should not remove the element
//
//    stack.push(2.718);
//    ASSERT_EQ(stack.peek(), 2.718);
//    ASSERT_EQ(stack.size(), 2);
//}
//
//TEST(StackTest, PeekOnEmptyStack) {
//    Stack<int> stack;
//    ASSERT_DEATH(stack.peek(), "isEmpty");
//
//
//    const Stack<int> constStack;
//    ASSERT_DEATH(constStack.peek(), "isEmpty");
//}
//
//TEST(StackTest, SizeAndIsEmpty) {
//    Stack<std::string> stack;
//    ASSERT_TRUE(stack.isEmpty());
//    ASSERT_EQ(stack.size(), 0);
//
//    stack.push("hello");
//    ASSERT_FALSE(stack.isEmpty());
//    ASSERT_EQ(stack.size(), 1);
//
//    stack.push("world");
//    ASSERT_EQ(stack.size(), 2);
//
//    stack.pop();
//    ASSERT_EQ(stack.size(), 1);
//
//    stack.pop();
//    ASSERT_TRUE(stack.isEmpty());
//    ASSERT_EQ(stack.size(), 0);
//}
//
//TEST(StackTest, AssignmentOperatorCopy) {
//    Stack<int> original{ 5, 6, 7 };
//    Stack<int> assigned;
//    assigned = original;
//
//    ASSERT_FALSE(assigned.isEmpty());
//    ASSERT_EQ(assigned.size(), 3);
//    ASSERT_EQ(assigned.peek(), 7);
//
//    original.push(8);
//    ASSERT_EQ(original.size(), 4);
//    ASSERT_EQ(original.peek(), 8);
//    ASSERT_EQ(assigned.size(), 3);
//    ASSERT_EQ(assigned.peek(), 7);
//
//    assigned = assigned; // Self-assignment
//    ASSERT_FALSE(assigned.isEmpty());
//    ASSERT_EQ(assigned.size(), 3);
//    ASSERT_EQ(assigned.peek(), 7);
//}
//
//TEST(StackTest, AssignmentOperatorMove) {
//    Stack<int> original{ 9, 10 };
//    Stack<int> assigned;
//    assigned = std::move(original);
//
//    ASSERT_TRUE(original.isEmpty());
//    ASSERT_EQ(original.size(), 0);
//    ASSERT_FALSE(assigned.isEmpty());
//    ASSERT_EQ(assigned.size(), 2);
//    ASSERT_EQ(assigned.peek(), 10);
//
//    assigned = std::move(assigned); // Self-move
//    ASSERT_FALSE(assigned.isEmpty());
//    ASSERT_EQ(assigned.size(), 2);
//    ASSERT_EQ(assigned.peek(), 10);
//}
//
//struct MoveOnly {
//    int value;
//    MoveOnly(int v) : value(v) {}
//    MoveOnly(MoveOnly&&) = default;
//    MoveOnly& operator=(MoveOnly&&) = default;
//
//    MoveOnly(const MoveOnly&) = delete;
//    MoveOnly& operator=(const MoveOnly&) = delete;
//};
//
//TEST(StackTest, MoveOnlyType) {
//    Stack<MoveOnly> stack;
//    stack.push(MoveOnly(42));
//    ASSERT_EQ(stack.size(), 1);
//}
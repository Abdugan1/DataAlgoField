#include <iostream>
#include <stack>
#include <vector>
#include <forward_list>
#include <list>

#include "Stack.h"
#include "Vector.h"

//struct MoveOnly {
//    int value;
//    MoveOnly(int v) : value(v) {}
//    MoveOnly(MoveOnly&&) = default;
//    MoveOnly& operator=(MoveOnly&&) = default;
//
//    MoveOnly(const MoveOnly&) = delete;
//    MoveOnly& operator=(const MoveOnly&) = delete;
//};


struct MoveOnly {
    int value;
    MoveOnly(int v) : value(v) {}
    MoveOnly(MoveOnly&&) noexcept = default;
    MoveOnly& operator=(MoveOnly&&) noexcept = default;

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    ~MoveOnly() {
        std::cout << "~MoveOnly(" << value << ")\n";
    }
};


class MyClass
{
public:
    MyClass()
        : val{0}
    {
        std::cout << "def MyClass " << val << std::endl;
    }
    MyClass(int a)
        : val{a}
    {
        std::cout << "MyClass(a) " << val << std::endl;
    }
    MyClass(const MyClass& other)
        : val(other.val)
    {
        std::cout << "Copy MyClass(other) " << val << std::endl;
    }

    MyClass(MyClass&& other) noexcept
        : val(other.val)
    {
        other.val = 0;
        std::cout << "Move MyClass(other) " << val << std::endl;
    }

    MyClass& operator=(const MyClass& other)
    {
        if (this == &other)
        {
            return *this;
        }

        val = other.val;

        std::cout << "Copy Operator " << val << std::endl;

        return *this;
    }

    MyClass& operator=(MyClass&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        val = other.val;
        other.val = 0;

        std::cout << "Move Operator " << val << std::endl;

        return *this;
    }

    ~MyClass()
    {
        std::cout << "~MyClass() " << val << std::endl;
    }
public:
    int val{};
};

int main()
{
    std::forward_list<int> forwardList;
    std::vector<int> vectorA;
    forwardList.push_front(123);

	return 0;
}
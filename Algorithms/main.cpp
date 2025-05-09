#include <iostream>
#include <stack>
#include <vector>

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
    Vector<MyClass> myVectorData{ 1, 2, 3 };
    for (auto& v : myVectorData)
    {
        std::cout << v.val << std::endl;
    }

    myVectorData.pushFront(123);

    for (auto& v : myVectorData)
    {
        std::cout << v.val << std::endl;
    }

    //int size = 3;
    //MyClass* data = new MyClass[10]{1, 2, 3, };
    //for (int i = 0; i < size; ++i)
    //{
    //    std::cout << data[i].val << std::endl;
    //}

    //for (int i = size; i > 0; --i)
    //{
    //    data[i] = std::move(data[i - 1]);
    //}

    //for (int i = 0; i < size + 1; ++i)
    //{
    //    std::cout << data[i].val << std::endl;
    //}

    //delete[] data;

    //std::vector<MoveOnly> vector{MoveOnly(42)};
    //vector.insert(vector.begin(), MoveOnly(32));
    //vector.begin();
    //std::cout << vector.capacity() << std::endl;
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //vector.push_back(MoveOnly(42));
    //std::cout << vector.capacity() << std::endl;
    //std::cout << vector.size() << std::endl;

    //Vector<int> a;
    //a.pushBack(123);
    //a.pushBack(123);
    //a.pushBack(123);
    //a.pushBack(123);
    //a.pushBack(123);
    //std::cout << a.size() << std::endl;

    //Vector<MoveOnly> a;
    //a.pushBack(MoveOnly(1));
    //a.pushBack(MoveOnly(2));
    //a.pushBack(MoveOnly(3));
    //a.popFront(); // Uh-oh


    //Vector<MoveOnly> myVector;
    //myVector.pushBack(MoveOnly(42));
    //std::cout << myVector.size() << std::endl;



    //Vector<std::unique_ptr<int>> myVectorMemory;
    //myVectorMemory.pushBack(std::make_unique<int>(42));
    //std::cout << myVectorMemory.size() << std::endl;

	return 0;
}
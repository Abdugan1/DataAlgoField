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

int main()
{
	std::cout << "Hello, World!\n";

    //std::vector<MoveOnly> vector;
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
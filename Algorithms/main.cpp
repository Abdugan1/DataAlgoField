#include <iostream>
#include <list>
#include <vector>

#include "SingleLinkedList.h"
#include "Vector.h"

template<typename T>
void printMyVector(const Vector<T>& vec)
{
	for (auto&& val : vec)
	{
		std::cout << val << " ";
	}
	std::cout << std::endl;
}

template<typename T>
void pushBack(T&& val)
{
	std::forward<T>(val);
}

int main()
{

	return 0;
}

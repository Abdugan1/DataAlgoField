#pragma once
#include "SingleLinkedList.h"

template <typename T>
class Stack final
{
public:
	Stack();
	Stack(std::initializer_list<T> vals);
	Stack(const Stack<T>& other);
	Stack(Stack<T>&& other) noexcept;
	~Stack();

	void pushBack(const T& val);
	void pushBack(T&& val);
	void popBack();

	T& peek();
	const T& peek() const;

	constexpr bool isEmpty() const noexcept;

	Stack<T>& operator=(const Stack<T>& other);
	Stack<T>& operator=(Stack<T>&& other) noexcept;


private:
	SingleLinkedList<T> data_;
};

template <typename T>
constexpr bool Stack<T>::isEmpty() const noexcept
{
	return data_.isEmpty();
}

template <typename T>
Stack<T>::Stack()
{
}

template <typename T>
Stack<T>::Stack(std::initializer_list<T> vals)
	: data_{vals}
{
}

template <typename T>
Stack<T>::Stack(const Stack<T>& other)
	: data_{other.data_}
{
}

template <typename T>
Stack<T>::Stack(Stack<T>&& other) noexcept
	: data_{std::move(other.data_)}
{
}

template <typename T>
Stack<T>::~Stack()
{
	// If this class data_'s destructor, then SingleLinkedList clears its data in the destructor
}

template <typename T>
void Stack<T>::pushBack(const T& val)
{
	data_.pushFront(val);
}

template <typename T>
void Stack<T>::pushBack(T&& val)
{
	data_.pushFront(std::move(val));
}

template <typename T>
void Stack<T>::popBack()
{
	data_.popFront();
}

template <typename T>
T& Stack<T>::peek()
{
	return data_.front();
}

template <typename T>
const T& Stack<T>::peek() const
{
	return data_.front();
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other)
{
	// A little bit redundant, I guess, since SingleLinkedList check for this.
	// But, just in case...
	if (this == &other)
	{
		return *this;
	}
	data_ = other.data_;

	return *this;
}

template <typename T>
Stack<T>& Stack<T>::operator=(Stack<T>&& other) noexcept
{
	// A little bit redundant, I guess, since SingleLinkedList check for this.
	// But, just in case...
	if (this == &other)
	{
		return *this;
	}
	data_ = std::move(other.data_);

	return *this;
}

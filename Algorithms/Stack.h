//#pragma once
//#include "SingleLinkedList.h"
//
//template <typename T>
//class Stack final
//{
//public:
//	Stack() = default;
//	Stack(std::initializer_list<T> vals);
//	Stack(const Stack<T>& other) = default;
//	Stack(Stack<T>&& other) noexcept = default;
//	~Stack() = default;
//
//	void push(const T& val);
//	void push(T&& val);
//	void pop();
//
//	T& peek();
//	const T& peek() const;
//
//	constexpr size_t size() const noexcept;
//	constexpr bool isEmpty() const noexcept;
//
//	Stack<T>& operator=(const Stack<T>& other);
//	Stack<T>& operator=(Stack<T>&& other) noexcept;
//
//private:
//	SingleLinkedList<T> data_;
//};
//
//template <typename T>
//constexpr size_t Stack<T>::size() const noexcept
//{
//	return data_.size();
//}
//
//template <typename T>
//constexpr bool Stack<T>::isEmpty() const noexcept
//{
//	return data_.isEmpty();
//}
//
//template <typename T>
//Stack<T>::Stack(std::initializer_list<T> vals)
//// Cannot use since the order will be reversed
//	// : data_{vals}
//{
//	for (auto&& val : vals)
//	{
//		push(val);
//	}
//}
//
//template <typename T>
//void Stack<T>::push(const T& val)
//{
//	data_.pushFront(val);
//}
//
//template <typename T>
//void Stack<T>::push(T&& val)
//{
//	data_.pushFront(std::move(val));
//}
//
//template <typename T>
//void Stack<T>::pop()
//{
//	data_.popFront();
//}
//
//template <typename T>
//T& Stack<T>::peek()
//{
//	return data_.front();
//}
//
//template <typename T>
//const T& Stack<T>::peek() const
//{
//	return data_.front();
//}
//
//template <typename T>
//Stack<T>& Stack<T>::operator=(const Stack<T>& other)
//{
//	// A little bit redundant, I guess, since SingleLinkedList check for this.
//	// But, just in case...
//	if (this == &other)
//	{
//		return *this;
//	}
//	data_ = other.data_;
//
//	return *this;
//}
//
//template <typename T>
//Stack<T>& Stack<T>::operator=(Stack<T>&& other) noexcept
//{
//	// A little bit redundant, I guess, since SingleLinkedList check for this.
//	// But, just in case...
//	if (this == &other)
//	{
//		return *this;
//	}
//	data_ = std::move(other.data_);
//
//	return *this;
//}

#pragma once
#include <algorithm>
#include <cassert>
#include <initializer_list>


template<typename T>
class Vector final
{
public:
	class Iterator
	{
	public:
		Iterator(T* ptr) : ptr_{ptr} {}

		T& operator*() { return *ptr_; }
		const T& operator*() const { return *ptr_; }

		Iterator& operator++() { ++ptr_; return *this; } // Pre-increment
		Iterator operator++(int) { Iterator tmp = *this; ++ptr_; return tmp; } // Post-increment

		constexpr bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
		constexpr bool operator!=(const Iterator& other) const { return !(*this == other); }
	private:
		T* ptr_{ nullptr };
	};

	class ConstIterator
	{
	public:
		ConstIterator(const T* ptr) : ptr_{ptr} {}

		const T& operator*() { return *ptr_; }

		ConstIterator& operator++() { ++ptr_; return *this; }
		ConstIterator operator++(int) { ConstIterator tmp = *this; ++ptr_; return tmp; }

		constexpr bool operator==(const ConstIterator& other) const { return ptr_ == other.ptr_; }
		constexpr bool operator!=(const ConstIterator& other) const { return !(*this == other); }

	private:
		const T* ptr_{ nullptr };
	};

public:
	Vector();
	Vector(std::initializer_list<T> vals);
	~Vector();

	Vector(const Vector<T>& other);
	Vector(Vector<T>&& other) noexcept;

	Vector& operator=(const Vector<T>& other);
	Vector& operator=(Vector<T>&& other) noexcept;

	void pushFront(const T& val);
	void pushFront(T&& val);

	void pushBack(const T& val);
	void pushBack(T&& val);

	void popFront();
	void popBack();

	void clear();

	constexpr size_t size() const noexcept;
	constexpr size_t capacity() const noexcept;
	constexpr bool isEmpty() const noexcept;

	T& operator[](size_t i);
	const T& operator[](size_t i) const;

	Iterator begin();
	Iterator end();

	ConstIterator begin() const;
	ConstIterator end() const;

private:
	void inflate();

	template<typename ValType>
	void insertAtBeginning(ValType&& val);

	template<typename ValType>
	void insertAtEnd(ValType&& val);

	void initializeEmptyVector();

	constexpr bool isInBounds(size_t i) const noexcept;

	void copyFromAnother(const Vector<T>& other);
	void moveFromAnother(Vector<T>&& other);

private:
	enum : size_t
	{
		DefaultCapacity = 8
	};

	T* data_{nullptr};

	size_t size_{0};
	size_t capacity_{ DefaultCapacity };
};


template <typename T>
constexpr bool Vector<T>::isInBounds(size_t i) const noexcept
{
	return (i < size_);
}


template <typename T>
Vector<T>::Vector()
{
	initializeEmptyVector();
}


template <typename T>
Vector<T>::Vector(std::initializer_list<T> vals)
{
	size_ = vals.size();
	if (size_ >= capacity_)
	{
		capacity_ = size_;
	}
	data_ = new T[capacity_];

	size_t i = 0;
	for (auto&& val : vals)
	{
		data_[i] = val;
		i++;
	}
}


template <typename T>
Vector<T>::~Vector()
{
	// Do not call clear()! It reinits an empty vector again!
	delete[] data_;
	data_ = nullptr;

	size_ = 0;
	capacity_ = 0;
}


template <typename T>
Vector<T>::Vector(const Vector<T>& other)
{
	copyFromAnother(other);
}


template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this == &other)
	{
		return *this;
	}

	copyFromAnother(other);

	return *this;
}


template <typename T>
Vector<T>::Vector(Vector<T>&& other) noexcept
{
	// Should I use std::move here? Is it treated like lvalue if I don't use std::move?
	moveFromAnother(std::move(other));
}


template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept
{
	// Do I need to do this? In cpy ctor, yes. But here?
	if (this == &other)
	{
		return *this;
	}

	// Should I use std::move here? Is it treated like lvalue if I don't use std::move?
	moveFromAnother(std::move(other));

	return *this;
}

template <typename T>
void Vector<T>::pushFront(const T& val)
{
	insertAtBeginning(val);
}

template <typename T>
void Vector<T>::pushFront(T&& val)
{
	insertAtBeginning(std::move(val));
}


template <typename T>
template <typename ValType>
void Vector<T>::insertAtBeginning(ValType&& val)
{
	const bool bHasEnoughCapacity = size_ + 1 <= capacity_;
	if (!bHasEnoughCapacity)
	{
		inflate();
	}

	for (size_t i = size_; i > 0; --i)
	{
		data_[i] = std::move(data_[i - 1]);
	}

	data_[0] = std::forward<ValType>(val);

	++size_;
}


template <typename T>
void Vector<T>::pushBack(const T& val)
{
	insertAtEnd(val);
}


template <typename T>
void Vector<T>::pushBack(T&& val)
{
	insertAtEnd(std::move(val));
}

template <typename T>
template <typename ValType>
void Vector<T>::insertAtEnd(ValType&& val)
{
	if (size_ >= capacity_)
	{
		inflate();
	}
	// We need std::forward. Notice the template ValType used. It's different from the container's
	data_[size_] = std::forward<ValType>(val);
	size_++;
}

template <typename T>
void Vector<T>::popFront()
{
	/**
	 * 1. emtpy
	 * 2. one elem
	 * 3. the usual
	 */

	assert(!isEmpty());

	for (size_t i = 0; i < size_ - 1; ++i)
	{
		data_[i] = std::move(data_[i + 1]);
	}

	--size_;
}


template <typename T>
void Vector<T>::popBack()
{
	assert(isInBounds(size_ - 1));
	--size_;
}


template <typename T>
void Vector<T>::clear()
{
	delete[] data_;
	data_ = nullptr;

	size_ = 0;
	capacity_ = 0;

	initializeEmptyVector();
}


template <typename T>
constexpr size_t Vector<T>::size() const noexcept
{
	return size_;
}


template <typename T>
constexpr size_t Vector<T>::capacity() const noexcept
{
	return capacity_;
}


template <typename T>
constexpr bool Vector<T>::isEmpty() const noexcept
{
	return size_ == 0;
}


template <typename T>
T& Vector<T>::operator[](size_t i)
{
	return const_cast<T&>(static_cast<const Vector&>(*this)[i]);
}


template <typename T>
const T& Vector<T>::operator[](size_t i) const
{
	assert(isInBounds(i));
	return data_[i];
}


template <typename T>
typename Vector<T>::Iterator Vector<T>::begin()
{
	return Iterator{ data_ };
}


template <typename T>
typename Vector<T>::Iterator Vector<T>::end()
{
	return Iterator{ data_ + size_ };
}


template <typename T>
typename Vector<T>::ConstIterator Vector<T>::begin() const
{
	return ConstIterator{ data_ };
}


template <typename T>
typename Vector<T>::ConstIterator Vector<T>::end() const
{
	return ConstIterator{ data_ + size_ };
}


template <typename T>
void Vector<T>::inflate()
{
	capacity_ *= 2;

	auto oldData = data_;

	data_ = new T[capacity_];

	for (size_t i = 0; i < size_; ++i)
	{
		data_[i] = std::move(oldData[i]);
	}

	delete[] oldData;
}


template <typename T>
void Vector<T>::initializeEmptyVector()
{
	size_ = 0;
	capacity_ = DefaultCapacity;
	data_ = new T[capacity_];
}


template <typename T>
void Vector<T>::copyFromAnother(const Vector<T>& other)
{
	T* newData = new T[other.capacity_];
	for (size_t i = 0; i < other.size_; ++i)
	{
		newData[i] = other.data_[i];
	}

	delete[] data_;
	data_ = newData;
	newData = nullptr;

	size_ = other.size_;
	capacity_ = other.capacity_;
}


template <typename T>
void Vector<T>::moveFromAnother(Vector<T>&& other)
{
	size_ = other.size_;
	capacity_ = other.capacity_;

	data_ = other.data_;

	// I could move one by one, but when you're allowed to "steal", why would you?
	/*data_ = new T[capacity_];
	for (size_t i = 0; i < size_; ++i)
	{
		data_[i] = std::move(other.data_[i]);
	}*/

	other.size_ = 0;
	other.capacity_ = 0;
	other.data_ = nullptr;
}


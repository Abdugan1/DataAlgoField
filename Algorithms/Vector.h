#pragma once
#include <algorithm>
#include <cassert>
#include <initializer_list>


template<typename T, typename Allocator = std::allocator<T>>
class Vector;

template<typename T, typename Allocator>
class ConstVectorIterator final
{
public:
	using val_type = T;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_iterator = ConstVectorIterator<T, Allocator>;
	using my_vector = Vector<T>;
	ConstVectorIterator(const my_vector* owner, const pointer ptr) :owner_{owner}, ptr_ { ptr } {}

	const T& operator*() { return *ptr_; }

	const_iterator& operator++() { ++ptr_; return *this; }
	const_iterator operator++(int) { const_iterator tmp = *this; ++ptr_; return tmp; }

	constexpr bool operator==(const const_iterator& other) const
	{
		assert(owner_ == other.owner_);
		return ptr_ == other.ptr_;
	}
	constexpr bool operator!=(const const_iterator& other) const { return !(*this == other); }

private:
	const my_vector* owner_{ nullptr };
	const T* ptr_{ nullptr };
};

template<typename T, typename Allocator>
class VectorIterator final
{
public:
	using val_type = T;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using iterator = VectorIterator<T, Allocator>;
	using my_vector = Vector<T>;

	VectorIterator(const my_vector* owner, pointer ptr) : owner_(owner), ptr_{ ptr } {}

	reference operator*() { return *ptr_; }
	const_reference operator*() const { return *ptr_; }

	iterator& operator++() { ++ptr_; return *this; } // Pre-increment
	iterator operator++(int) { iterator tmp = *this; ++ptr_; return tmp; } // Post-increment

	constexpr bool operator==(const iterator& other) const
	{
		assert(owner_ == other.owner_);
		return ptr_ == other.ptr_;
	}
	constexpr bool operator!=(const iterator& other) const { return !(*this == other); }
private:
	const my_vector* owner_{ nullptr };
	pointer ptr_{ nullptr };
};

template<typename T, typename Allocator>
class Vector final
{
public:
	using Iterator = VectorIterator<T, Allocator>;
	using ConstIterator = ConstVectorIterator<T, Allocator>;

	using allocator_type = Allocator;

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
		DefaultCapacity = 0
	};

	allocator_type allocator_{};

	T* data_{nullptr};

	size_t size_{0};
	size_t capacity_{ DefaultCapacity };
};

template <typename T, typename Allocator>
constexpr bool Vector<T, Allocator>::isInBounds(size_t i) const noexcept
{
	return (i < size_);
}


template <typename T, typename Allocator>
Vector<T, Allocator>::Vector()
{
}


template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(std::initializer_list<T> vals)
{
	size_ = vals.size();
	capacity_ = size_;
	data_ = allocator_.allocate(capacity_);

	size_t i = 0;
	for (auto& val : vals)
	{
		allocator_.construct(&data_[i], val);
		i++;
	}
}


template <typename T, typename Allocator>
Vector<T, Allocator>::~Vector()
{
	clear();
}


template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector<T>& other)
{
	copyFromAnother(other);
}


template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector<T>& other)
{
	if (this == &other)
	{
		return *this;
	}

	copyFromAnother(other);

	return *this;
}


template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector<T>&& other) noexcept
{
	moveFromAnother(std::move(other));
}


template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector<T>&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}

	moveFromAnother(std::move(other));

	return *this;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::pushFront(const T& val)
{
	insertAtBeginning(val);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::pushFront(T&& val)
{
	insertAtBeginning(std::move(val));
}


template <typename T, typename Allocator>
template <typename ValType>
void Vector<T, Allocator>::insertAtBeginning(ValType&& val)
{
	const bool bHasEnoughCapacity = size_ + 1 <= capacity_;
	if (!bHasEnoughCapacity)
	{
		inflate();
	}

	for (size_t i = size_; i > 0; --i)
	{
		allocator_.destroy(&data_[i]);
		allocator_.construct(&data_[i], std::move(data_[i - 1]));
	}

	allocator_.construct(&data_[0], std::forward<ValType>(val));

	++size_;
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::pushBack(const T& val)
{
	insertAtEnd(val);
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::pushBack(T&& val)
{
	insertAtEnd(std::move(val));
}

template <typename T, typename Allocator>
template <typename ValType>
void Vector<T, Allocator>::insertAtEnd(ValType&& val)
{
	if (size_ >= capacity_)
	{
		inflate();
	}

	allocator_.construct(&data_[size_], std::forward<ValType>(val));

	size_++;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::popFront()
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


template <typename T, typename Allocator>
void Vector<T, Allocator>::popBack()
{
	assert(isInBounds(size_ - 1));
	--size_;
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::clear()
{
	for (size_t i = 0; i < size_; ++i)
	{
		allocator_.destroy(&data_[i]);
	}

	allocator_.deallocate(data_, capacity_);

	data_ = nullptr;

	size_ = 0;
	capacity_ = 0;
}


template <typename T, typename Allocator>
constexpr size_t Vector<T, Allocator>::size() const noexcept
{
	return size_;
}


template <typename T, typename Allocator>
constexpr size_t Vector<T, Allocator>::capacity() const noexcept
{
	return capacity_;
}


template <typename T, typename Allocator>
constexpr bool Vector<T, Allocator>::isEmpty() const noexcept
{
	return size_ == 0;
}


template <typename T, typename Allocator>
T& Vector<T, Allocator>::operator[](size_t i)
{
	return const_cast<T&>(static_cast<const Vector&>(*this)[i]);
}


template <typename T, typename Allocator>
const T& Vector<T, Allocator>::operator[](size_t i) const
{
	assert(isInBounds(i));
	return data_[i];
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::begin()
{
	return Iterator(this, data_);
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::end()
{
	return Iterator( this, data_ + size_ );
}


template <typename T, typename Allocator>
typename Vector<T, Allocator>::ConstIterator Vector<T, Allocator>::begin() const
{
	return ConstIterator( this, data_ );
}


template <typename T, typename Allocator>
typename Vector<T, Allocator>::ConstIterator Vector<T, Allocator>::end() const
{
	return ConstIterator( this, data_ + size_ );
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::inflate()
{
	size_t oldCapacity = capacity_;

	if (capacity_ == 0)
	{
		capacity_ = 1;
	}
	else
	{
		capacity_ *= 2;
	}

	auto oldData = data_;

	data_ = allocator_.allocate(capacity_);

	for (size_t i = 0; i < size_; ++i)
	{
		allocator_.construct(&data_[i], std::move(oldData[i]));
		allocator_.destroy(&oldData[i]); // TOSEARCH: Do I even need this if I'm deallocating?
	}

	allocator_.deallocate(oldData, oldCapacity);
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::initializeEmptyVector()
{
	size_ = 0;
	capacity_ = DefaultCapacity;

	data_ = allocator_.allocate(capacity_);
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::copyFromAnother(const Vector<T>& other)
{
	clear();

	data_ = allocator_.allocate(other.capacity_);
	for (size_t i = 0; i < other.size_; ++i)
	{
		allocator_.construct(&data_[i], other.data_[i]);
	}

	size_ = other.size_;
	capacity_ = other.capacity_;
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::moveFromAnother(Vector<T>&& other)
{
	clear();

	size_ = other.size_;
	capacity_ = other.capacity_;

	data_ = other.data_;

	other.size_ = 0;
	other.capacity_ = 0;
	other.data_ = nullptr;
}


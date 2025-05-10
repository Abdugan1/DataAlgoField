#pragma once
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <memory>


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

	const_reference operator*() const { return *ptr_; }

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
	using alloc_traits = std::allocator_traits<Allocator>;
	using pointer = typename alloc_traits::pointer;
	using size_type = typename alloc_traits::size_type;

public:
	Vector() = default;
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

	void reset();

	void reserve(size_type n);

	constexpr size_type size() const noexcept { return size_; }
	constexpr size_type capacity() const noexcept { return capacity_; }
	constexpr bool isEmpty() const noexcept;

	T& operator[](size_type i);
	const T& operator[](size_type i) const;

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

	constexpr bool isInBounds(size_type i) const noexcept;

	void copyFromAnother(const Vector<T>& other);
	void moveFromAnother(Vector<T>&& other);

private:
	allocator_type allocator_{};

	pointer data_{nullptr};

	size_type size_{0};
	size_type capacity_{ 0 };
};

template <typename T, typename Allocator>
constexpr bool Vector<T, Allocator>::isInBounds(size_type i) const noexcept
{
	return (i < size_);
}


template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(std::initializer_list<T> vals)
{
	size_ = vals.size();
	capacity_ = size_;
	data_ = alloc_traits::allocate(allocator_, capacity_);

	size_type i = 0;
	for (auto& val : vals)
	{
		alloc_traits::construct(allocator_, &data_[i], val);
		i++;
	}
}


template <typename T, typename Allocator>
Vector<T, Allocator>::~Vector()
{
	reset();
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

	/* Consider using this? */
	/*
	* std::move_backward(data_, data_ + size_, data_ + size_ + 1);
	*/
	for (size_type i = size_; i > 0; --i)
	{
		/*
		* Since we're dealing with raw memory, we cannot use plain data_[i] = std::move(data_[i - 1]).
		* Notice that the iteration beginning is i = size_, this is out of range actually.
		* When you use plain new and delete, mostly when you Vector::reserve, the out-of-range side,
		* i.e., the range above size_, is valid, either by Default constructor or something else.
		* But when you deal with raw memory, allocation and construction is separated. So it's not valid.
		* When we shift, we need to construct every time to prevent access/assigning of not valid data.
		* When you finish constructing, you must delete the old/moved-from object to keep the logic 
		* to shift to invalid data. When you delete the old data, the place is invalid,
		* hence, the same logic - calling the Move constructor - can be applied again.
		*/
		alloc_traits::construct(allocator_, &data_[i], std::move(data_[i - 1]));
		alloc_traits::destroy(allocator_, &data_[i - 1]); 
	}

	alloc_traits::construct(allocator_, &data_[0], std::forward<ValType>(val));

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

	alloc_traits::construct(allocator_, &data_[size_], std::forward<ValType>(val));

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

	alloc_traits::destroy(allocator_, &data_[0]);

	for (size_type i = 0; i < size_ - 1; ++i)
	{
		data_[i] = std::move(data_[i + 1]);
	}

	--size_;
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::popBack()
{
	assert(isInBounds(size_ - 1));
	alloc_traits::destroy(allocator_, &data_[size_ - 1]);
	--size_;
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::reset()
{
	for (size_type i = 0; i < size_; ++i)
	{
		alloc_traits::destroy(allocator_, &data_[i]);
	}

	alloc_traits::deallocate(allocator_, data_, capacity_);

	data_ = nullptr;

	size_ = 0;
	capacity_ = 0;
}


template <typename T, typename Allocator>
constexpr bool Vector<T, Allocator>::isEmpty() const noexcept
{
	return size_ == 0;
}


template <typename T, typename Allocator>
T& Vector<T, Allocator>::operator[](size_type i)
{
	return const_cast<T&>(static_cast<const Vector&>(*this)[i]);
}


template <typename T, typename Allocator>
const T& Vector<T, Allocator>::operator[](size_type i) const
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
	size_type neededCapacity = capacity_;

	if (neededCapacity == 0)
	{
		neededCapacity = 1;
	}
	else
	{
		neededCapacity *= 2;
	}

	reserve(neededCapacity);
}


template<typename T, typename Allocator>
void Vector<T, Allocator>::reserve(size_type n)
{
	if (n < capacity_)
	{
		return;
	}

	size_type oldCapacity = capacity_;

	auto oldData = data_;

	data_ = alloc_traits::allocate(allocator_, n);

	for (size_type i = 0; i < size_; ++i)
	{
		alloc_traits::construct(allocator_, &data_[i], std::move(oldData[i]));
		alloc_traits::destroy(allocator_, &oldData[i]);

		/* I could use this? */
		/*
		* std::uninitialized_move(oldData, oldData + size_, data_);
		* std::destroy(oldData, oldData + size_);
		*/
	}

	capacity_ = n;
	alloc_traits::deallocate(allocator_, oldData, oldCapacity);
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::copyFromAnother(const Vector<T>& other)
{
	reset();

	data_ = alloc_traits::allocate(allocator_, other.capacity_); 

	for (size_type i = 0; i < other.size_; ++i)
	{
		alloc_traits::construct(allocator_, &data_[i], other.data_[i]);
	}

	size_ = other.size_;
	capacity_ = other.capacity_;
}


template <typename T, typename Allocator>
void Vector<T, Allocator>::moveFromAnother(Vector<T>&& other)
{
	reset();

	size_ = other.size_;
	capacity_ = other.capacity_;

	data_ = other.data_;

	other.size_ = 0;
	other.capacity_ = 0;
	other.data_ = nullptr;
}


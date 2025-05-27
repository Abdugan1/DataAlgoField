#pragma once
#include <algorithm>
#include <cassert>

template<typename T, typename Allocator>
class DoubleLinkedListIterator;

template<typename T, typename Allocator>
class DoubleLinkedListConstIterator;

template<typename ValType>
struct DoubleLinkedListNode final
{
	using Node = DoubleLinkedListNode;
	using NodePtr = Node*;

	ValType value;
	NodePtr previous{ nullptr };
	NodePtr next{ nullptr };

	DoubleLinkedListNode() = default;

	DoubleLinkedListNode(const ValType& val, NodePtr previousNode = nullptr, NodePtr nextNode = nullptr)
		: value(val), previous(previousNode), next(nextNode)
	{
	}

	DoubleLinkedListNode(ValType&& val, NodePtr previousNode = nullptr, NodePtr nextNode = nullptr)
		: value(std::move(val)), previous(previousNode), next(nextNode)
	{
	}

	template<typename Alloc>
	void freeNode(Alloc& allocator)
	{
		using AllocTraits = std::allocator_traits<Alloc>;

		AllocTraits::destroy(allocator, this);
		AllocTraits::deallocate(allocator, this, 1);
	}
};


template<typename T, typename Allocator = std::allocator<T>>
class DoubleLinkedList final
{
public:
	using Node = DoubleLinkedListNode<T>;
	using NodePtr = Node*;
	using Iterator = DoubleLinkedListIterator<T, Allocator>;
	using ConstIterator = DoubleLinkedListConstIterator<T, Allocator>;

	using AllocType = Allocator;
	using AllocTypeTraits = std::allocator_traits<AllocType>;
	using AllocNode = typename AllocTypeTraits::template rebind_alloc<Node>;
	using AllocNodeTraits = std::allocator_traits<AllocNode>;

public:
	DoubleLinkedList();
	DoubleLinkedList(std::initializer_list<T> vals);
	DoubleLinkedList(const DoubleLinkedList& other);
	DoubleLinkedList(DoubleLinkedList&& other) noexcept;
	~DoubleLinkedList();

	void pushFront(const T& val);
	void pushFront(T&& val);

	void pushBack(const T& val);
	void pushBack(T&& val);

	void popFront();
	void popBack();

	void clear();

	void remove(Iterator where);

	constexpr size_t size() const noexcept;
	constexpr bool isEmpty() const noexcept;

	DoubleLinkedList& operator=(const DoubleLinkedList& other);
	DoubleLinkedList& operator=(DoubleLinkedList&& other) noexcept;

	T& front();
	const T& front() const;

	T& back();
	const T& back() const;

	Iterator begin();
	Iterator end();

	ConstIterator begin() const;
	ConstIterator end() const;

	ConstIterator cbegin() const;
	ConstIterator cend() const;

private:
	constexpr bool isInBounds(size_t index) const noexcept;

	void copyFromAnother(const DoubleLinkedList& other);
	void moveFromAnother(DoubleLinkedList&& other);

	template<typename ValType>
	void insertAtBeginning(ValType&& val);

	template<typename ValType>
	void insertAtEnd(ValType&& val);

	template<typename... Args>
	NodePtr allocateAndConstruct(Args&&... args);

	Node* get(size_t index);
	const Node* get(size_t index) const;

private:
	AllocType allocator_{};
	AllocNode nodeAllocator_{};

	NodePtr head_{ nullptr };
	NodePtr tail_{ nullptr };
	size_t size_{ 0 };
};


template<typename T, typename Allocator>
class DoubleLinkedListIterator
{
public:
	using Node = DoubleLinkedListNode<T>;
	using NodePtr = Node*;
	using Iterator = DoubleLinkedListIterator;

	using MyList = DoubleLinkedList<T, Allocator>;
public:
	DoubleLinkedListIterator(const MyList* owner, Node* node) : owner_{ owner }, node_{ node } {}

	T& operator*() { return node_->value; }
	const T& operator*() const { return node_->value; }

	Iterator& operator++() { node_ = node_->next; return *this; }
	Iterator operator++(int) { Iterator old(owner_, node_); node_ = node_->next; return old; }

	constexpr bool operator==(const Iterator& other) const
	{
		assert(owner_ == other.owner_);
		return node_ == other.node_;
	}
	constexpr bool operator!=(const Iterator& other) const { return !(*this == other); }

private:
	const MyList* owner_ = nullptr;
	NodePtr node_{ nullptr };

	friend class MyList;
};

template<typename T, typename Allocator>
class DoubleLinkedListConstIterator
{
public:
	using Node = DoubleLinkedListNode<T>;
	using NodePtr = Node*;
	using ConstIterator = DoubleLinkedListConstIterator;

	using MyList = DoubleLinkedList<T, Allocator>;
public:
	DoubleLinkedListConstIterator(const MyList* owner, Node* node) : owner_{ owner }, node_{ node } {}

	const T& operator*() const { return node_->value; }

	ConstIterator& operator++() { node_ = node_->next; return *this; }
	ConstIterator operator++(int) { ConstIterator old(owner_, node_); node_ = node_->next; return old; }

	constexpr bool operator==(const ConstIterator& other) const
	{
		assert(owner_ == other.owner_);
		return node_ == other.node_;
	}
	constexpr bool operator!=(const ConstIterator& other) const { return !(*this == other); }

private:
	const MyList* owner_{ nullptr };
	NodePtr node_{ nullptr };
};


template <typename T, typename Allocator>
constexpr size_t DoubleLinkedList<T, Allocator>::size() const noexcept
{
	return size_;
}

template <typename T, typename Allocator>
constexpr bool DoubleLinkedList<T, Allocator>::isEmpty() const noexcept
{
	return size_ == 0;
}


template <typename T, typename Allocator>
constexpr bool DoubleLinkedList<T, Allocator>::isInBounds(size_t index) const noexcept
{
	return index < size_;
}

template <typename T, typename Allocator>
DoubleLinkedList<T, Allocator>::DoubleLinkedList()
{
}

template <typename T, typename Allocator>
DoubleLinkedList<T, Allocator>::DoubleLinkedList(const DoubleLinkedList& other)
{
	copyFromAnother(other);
}

template <typename T, typename Allocator>
DoubleLinkedList<T, Allocator>::DoubleLinkedList(std::initializer_list<T> vals)
{
	size_ = vals.size();
	NodePtr previous = nullptr;
	for (auto& val : vals)
	{
		NodePtr node = allocateAndConstruct(val);

		if (head_ == nullptr)
		{
			head_ = node;
		}

		if (previous)
		{
			previous->next = node;
			node->previous = previous;
		}

		previous = node;
	}

	tail_ = previous;
}

template <typename T, typename Allocator>
DoubleLinkedList<T, Allocator>::DoubleLinkedList(DoubleLinkedList&& other) noexcept
{
	moveFromAnother(std::move(other));
}

template <typename T, typename Allocator>
DoubleLinkedList<T, Allocator>::~DoubleLinkedList()
{
	clear();
}

template <typename T, typename Allocator>
typename DoubleLinkedList<T, Allocator>::Node* DoubleLinkedList<T, Allocator>::get(size_t index)
{
	return const_cast<Node*>(static_cast<const DoubleLinkedList&>(*this).get(index));
}

template <typename T, typename Allocator>
const typename DoubleLinkedList<T, Allocator>::Node* DoubleLinkedList<T, Allocator>::get(size_t index) const
{
	if (!isInBounds(index))
	{
		return nullptr;
	}

	Node* node = head_;
	for (size_t i = 0; i < index; ++i)
	{
		node = node->next;
	}

	return node;
}

template <typename T, typename Allocator>
void DoubleLinkedList<T, Allocator>::pushFront(const T& val)
{
	insertAtBeginning(val);
}

template <typename T, typename Allocator>
void DoubleLinkedList<T, Allocator>::pushFront(T&& val)
{
	insertAtBeginning(std::move(val));
}

template <typename T, typename Allocator>
template <typename ValType>
void DoubleLinkedList<T, Allocator>::insertAtBeginning(ValType&& val)
{
	/**
	 * 1. empty
	 * 2. one elem
	 * 3. the usual
	 */

	auto newNode = allocateAndConstruct(std::forward<ValType>(val));

	if (isEmpty())
	{
		head_ = newNode;
		tail_ = head_;
	}
	else
	{
		head_->previous = newNode;
		newNode->next = head_;
		head_ = newNode;
	}

	++size_;
}


template <typename T, typename Allocator>
void DoubleLinkedList<T, Allocator>::pushBack(const T& val)
{
	insertAtEnd(val);
}

template <typename T, typename Allocator>
void DoubleLinkedList<T, Allocator>::pushBack(T&& val)
{
	insertAtEnd(std::move(val));
}

template <typename T, typename Allocator>
template <typename ValType>
void DoubleLinkedList<T, Allocator>::insertAtEnd(ValType&& val)
{
	/**
	 * 1. empty
	 * 2. one elem
	 * 3. the usual
	 */

	NodePtr newNode = allocateAndConstruct(std::forward<ValType>(val));

	if (isEmpty())
	{
		head_ = newNode;
		tail_ = head_;
	}
	else
	{
		tail_->next = newNode;
		newNode->previous = tail_;
		tail_ = newNode;
	}

	++size_;
}

template<typename T, typename Allocator>
template<typename ...Args>
typename DoubleLinkedList<T, Allocator>::NodePtr DoubleLinkedList<T, Allocator>::allocateAndConstruct(Args&&... args)
{
	NodePtr node = AllocNodeTraits::allocate(nodeAllocator_, 1);
	AllocNodeTraits::construct(nodeAllocator_, node, std::forward<Args>(args)...);
	return node;
}

template <typename T, typename Allocator>
void DoubleLinkedList<T, Allocator>::popFront()
{
	/**
	 * 1. empty
	 * 2. one elem
	 * 3. the usual
	 */

	assert(!isEmpty());

	if (head_ == tail_)
	{
		head_->freeNode(nodeAllocator_);
		head_ = nullptr;
		tail_ = nullptr;
	}
	else
	{
		NodePtr next = head_->next;
		next->previous = nullptr;

		head_->freeNode(nodeAllocator_);
		head_ = next;
	}

	--size_;
}

template <typename T, typename Allocator>
void DoubleLinkedList<T, Allocator>::popBack()
{
	/**
	* 1. empty
	* 2. one elem
	* 3. the usual
	*/

	assert(!isEmpty());

	if (head_ == tail_)
	{
		head_->freeNode(nodeAllocator_);
		head_ = nullptr;
		tail_ = nullptr;
	}
	else
	{
		NodePtr previous = tail_->previous;
		previous->next = nullptr;

		tail_->freeNode(nodeAllocator_);
		tail_ = previous;
	}

	--size_;
}


template <typename T, typename Allocator>
void DoubleLinkedList<T, Allocator>::clear()
{
	NodePtr node = head_;
	while (node)
	{
		NodePtr next = node->next;
		node->freeNode(nodeAllocator_);
		node = next;
	}

	head_ = nullptr;
	tail_ = nullptr;
	size_ = 0;
}

template <typename T, typename Allocator>
void DoubleLinkedList<T, Allocator>::remove(Iterator where)
{
	/**
	 * 1. empty
	 * 2. one elem
	 * 3. beginning
	 * 4. end
	 * 5. 
	 */
	assert(where != end());


	NodePtr previousNode = where.node_->previous;
	NodePtr toDeleteNode = where.node_;
	NodePtr nextNode = where.node_->next;

	if (toDeleteNode == head_)
	{
		popFront();
	}
	else if (toDeleteNode == tail_)
	{
		popBack();
	}
	else
	{
		previousNode->next = nextNode;
		toDeleteNode->freeNode(nodeAllocator_);
		--size_;
	}
}


template<typename T, typename Allocator>
DoubleLinkedList<T, Allocator>& DoubleLinkedList<T, Allocator>::operator=(const DoubleLinkedList& other)
{
	if (this == &other)
	{
		return *this;
	}
	copyFromAnother(other);

	return *this;
}


template<typename T, typename Allocator>
DoubleLinkedList<T, Allocator>& DoubleLinkedList<T, Allocator>::operator=(DoubleLinkedList&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	moveFromAnother(std::move(other));

	return *this;
}


template <typename T, typename Allocator>
T& DoubleLinkedList<T, Allocator>::front()
{
	return const_cast<T&>(static_cast<const DoubleLinkedList&>(*this).front());
}

template <typename T, typename Allocator>
const T& DoubleLinkedList<T, Allocator>::front() const
{
	assert(!isEmpty());
	return head_->value;
}

template <typename T, typename Allocator>
T& DoubleLinkedList<T, Allocator>::back()
{
	return const_cast<T&>(static_cast<const DoubleLinkedList&>(*this).back());
}

template <typename T, typename Allocator>
const T& DoubleLinkedList<T, Allocator>::back() const
{
	assert(!isEmpty());
	return tail_->value;
}

template <typename T, typename Allocator>
typename DoubleLinkedList<T, Allocator>::Iterator DoubleLinkedList<T, Allocator>::begin()
{
	return Iterator(this, head_);
}

template <typename T, typename Allocator>
typename DoubleLinkedList<T, Allocator>::Iterator DoubleLinkedList<T, Allocator>::end()
{
	return Iterator(this, nullptr);
}

template <typename T, typename Allocator>
typename DoubleLinkedList<T, Allocator>::ConstIterator DoubleLinkedList<T, Allocator>::begin() const
{
	return ConstIterator(this, head_);
}

template <typename T, typename Allocator>
typename DoubleLinkedList<T, Allocator>::ConstIterator DoubleLinkedList<T, Allocator>::end() const
{
	return ConstIterator(this, nullptr);
}

template <typename T, typename Allocator>
typename DoubleLinkedList<T, Allocator>::ConstIterator DoubleLinkedList<T, Allocator>::cbegin() const
{
	return begin();
}

template <typename T, typename Allocator>
typename DoubleLinkedList<T, Allocator>::ConstIterator DoubleLinkedList<T, Allocator>::cend() const
{
	return end();
}

template <typename T, typename Allocator>
void DoubleLinkedList<T, Allocator>::copyFromAnother(const DoubleLinkedList& other)
{
	if (!isEmpty())
	{
		clear();
	}

	NodePtr otherNode = other.head_;

	NodePtr previousNode = nullptr;
	while (otherNode)
	{
		NodePtr newNode = allocateAndConstruct(otherNode->value);

		if (head_ == nullptr)
		{
			head_ = newNode;
		}

		if (previousNode)
		{
			previousNode->next = newNode;
			newNode->previous = previousNode;
		}

		previousNode = newNode;
		otherNode = otherNode->next;
	}

	tail_ = previousNode;
	size_ = other.size_;
}

template <typename T, typename Allocator>
void DoubleLinkedList<T, Allocator>::moveFromAnother(DoubleLinkedList&& other)
{
	if (!isEmpty())
	{
		clear();
	}

	head_ = std::exchange(other.head_, nullptr);
	tail_ = std::exchange(other.tail_, nullptr);
	size_ = std::exchange(other.size_, 0);
}

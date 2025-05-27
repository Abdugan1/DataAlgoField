#pragma once
#include <algorithm>
#include <assert.h>
#include <initializer_list>


template<typename ValType>
struct SingleLinkedListNode final
{
	using Node = SingleLinkedListNode<ValType>;
	using NodePtr = Node*;

	ValType value;
	NodePtr next{ nullptr };

	SingleLinkedListNode() = default;

	SingleLinkedListNode(const ValType& val, NodePtr nextNode = nullptr)
		: value{ val }, next{ nextNode }
	{
	}

	SingleLinkedListNode(ValType&& val, NodePtr nextNode = nullptr)
		: value{ std::move(val) }, next{ nextNode }
	{
	}
	template<typename NodeAlloc>
	void freeNode(NodeAlloc& nodeAllocator)
	{
		using NodeAllocTraits = std::allocator_traits<NodeAlloc>;

		NodeAllocTraits::destroy(nodeAllocator, this);
		NodeAllocTraits::deallocate(nodeAllocator, this, 1);
	}
};

template<typename T, typename Allocator>
class SingleLinkedListIterator;


template<typename T, typename Allocator>
class ConstSingleLinkedListIterator;


template<typename T, typename Allocator = std::allocator<T>>
class SingleLinkedList final
{
public:
	using Node = SingleLinkedListNode<T>;
	using NodePtr = Node*;
	using Iterator = SingleLinkedListIterator<T, Allocator>;
	using ConstIterator = ConstSingleLinkedListIterator<T, Allocator>;

	using allocator_type = Allocator;
	using alloc_traits = std::allocator_traits<allocator_type>;
	using node_alloc_type = typename alloc_traits::template rebind_alloc<Node>;
	using node_alloc_traits = std::allocator_traits<node_alloc_type>;

	using node_pointer = typename node_alloc_traits::pointer;
	using size_type = typename alloc_traits::size_type;

public:
	SingleLinkedList() = default;
	SingleLinkedList(std::initializer_list<T> vals);
	SingleLinkedList(const SingleLinkedList& other);
	SingleLinkedList(SingleLinkedList&& other) noexcept;
	~SingleLinkedList();

	void pushFront(const T& val);
	void pushFront(T&& val);

	void pushBack(const T& val);
	void pushBack(T&& val);

	void popFront();
	void popBack();

	void clear();

	constexpr size_t size() const noexcept;
	constexpr bool isEmpty() const noexcept;

	SingleLinkedList& operator=(const SingleLinkedList& other);
	SingleLinkedList& operator=(SingleLinkedList&& other) noexcept;

	void remove(Iterator where);

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
	void copyFromAnother(const SingleLinkedList<T>& other);
	void moveFromAnother(SingleLinkedList<T>&& other);

	template<typename ValType>
	void insertAtBeginning(ValType&& val);

	template<typename ValType>
	void insertAtEnd(ValType&& val);

	Node* get(size_t index);
	const Node* get(size_t index) const;

	constexpr bool isInBound(size_t index) const noexcept;

private:
	allocator_type allocator_{};
	node_alloc_type nodeAllocator_{};

	Node* head_{ nullptr };
	Node* tail_{ nullptr };
	size_t size_{0};
};


template<typename T, typename Allocator>
class ConstSingleLinkedListIterator final
{
public:
	using val_type = T;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;

	using Iterator = ConstSingleLinkedListIterator<T, Allocator>;
	using Node = SingleLinkedListNode<T>;
	using NodePtr = Node*;

	using my_list = SingleLinkedList<T, Allocator>;

public:
	ConstSingleLinkedListIterator(const my_list* owner, NodePtr node) : owner_{ owner }, node_{ node } {}

	const_reference operator*() const { return node_->value; }

	Iterator& operator++() { node_ = node_->next; return *this; }
	Iterator operator++(int) { Iterator old(owner_, node_); node_ = node_->next; return old; }

	constexpr bool operator==(const Iterator& other) const
	{
		assert(owner_ == other.owner_);
		return node_ == other.node_;
	}
	constexpr bool operator!=(const Iterator& other) const { return !(*this == other); }

private:
	const my_list* owner_{ nullptr };
	NodePtr node_{ nullptr };

	friend class my_list;
};


template<typename T, typename Allocator>
class SingleLinkedListIterator final
{
public:
	using val_type = T;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;

	using Iterator = SingleLinkedListIterator<T, Allocator>;
	using Node = SingleLinkedListNode<T>;
	using NodePtr = Node*;

	using my_list = SingleLinkedList<T, Allocator>;

public:
	SingleLinkedListIterator(const my_list* owner, NodePtr node) : owner_{ owner }, node_{ node } {}

	reference operator*() { return node_->value; }
	const_reference operator*() const { return node_->value; }

	Iterator& operator++() { node_ = node_->next; return *this; }
	Iterator operator++(int) { Iterator old(owner_, node_); node_ = node_->next; return old; }

	constexpr bool operator==(const Iterator& other) const
	{
		assert(owner_ == other.owner_);
		return node_ == other.node_;
	}
	constexpr bool operator!=(const Iterator& other) const { return !(*this == other); }

private:
	const my_list* owner_{ nullptr };
	NodePtr node_{ nullptr };

	friend class my_list;
};


template <typename T, typename Allocator>
constexpr size_t SingleLinkedList<T, Allocator>::size() const noexcept
{
	return size_;
}


template <typename T, typename Allocator>
constexpr bool SingleLinkedList<T, Allocator>::isEmpty() const noexcept
{
	return size_ == 0;
}


template <typename T, typename Allocator>
constexpr bool SingleLinkedList<T, Allocator>::isInBound(size_t index) const noexcept
{
	return index < size_;
}


template <typename T, typename Allocator>
SingleLinkedList<T, Allocator>::SingleLinkedList(std::initializer_list<T> vals)
{
	size_ = vals.size();
	NodePtr previousNode = nullptr;

	for (auto& val : vals)
	{
		NodePtr newNode = node_alloc_traits::allocate(nodeAllocator_, 1);
		node_alloc_traits::construct(nodeAllocator_, newNode, val);

		if (head_ == nullptr)
		{
			head_ = newNode;
		}

		if (previousNode)
		{
			previousNode->next = newNode;
		}

		previousNode = newNode;
	}

	tail_ = previousNode;
}


template <typename T, typename Allocator>
SingleLinkedList<T, Allocator>::SingleLinkedList(const SingleLinkedList& other)
{
	copyFromAnother(other);
}


template <typename T, typename Allocator>
SingleLinkedList<T, Allocator>::SingleLinkedList(SingleLinkedList&& other) noexcept
{
	moveFromAnother(std::move(other));
}


template <typename T, typename Allocator>
SingleLinkedList<T, Allocator>::~SingleLinkedList()
{
	clear();
}


template <typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::pushFront(const T& val)
{
	insertAtBeginning(val);
}


template <typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::pushFront(T&& val)
{
	insertAtBeginning(std::move(val));
}


template <typename T, typename Allocator>
template <typename ValType>
void SingleLinkedList<T, Allocator>::insertAtBeginning(ValType&& val)
{
	Node* newNode = new Node(std::forward<ValType>(val), head_);
	head_ = newNode;

	if (size_ == 0)
	{
		tail_ = head_;
	}

	++size_;
}


template <typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::pushBack(const T& val)
{
	insertAtEnd(val);
}


template <typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::pushBack(T&& val)
{
	insertAtEnd(val);
}


template <typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::popFront()
{
	/**
	 * 1. empty
	 * 2. one element
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
		NodePtr toDelete = head_;
		head_ = head_->next;
		toDelete->freeNode(nodeAllocator_);
	}

	--size_;
}


template <typename T, typename Allocator>
template <typename ValType>
void SingleLinkedList<T, Allocator>::insertAtEnd(ValType&& val)
{
	/**
 * Cases:
 * 1. Head = Tail = nullptr -> empty
 * 2. Head != nullptr && Tail != nullptr
 */

	Node* newNode = new Node;
	newNode->value = std::forward<ValType>(val);
	newNode->next = nullptr;

	/** case 1 */
	if (head_ == nullptr && tail_ == nullptr)
	{
		head_ = newNode;
		tail_ = newNode;
	}

	/** case 2 */
	else if (head_ != nullptr && tail_ != nullptr)
	{
		tail_->next = newNode;
		tail_ = newNode;
	}

	++size_;
}


template <typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::popBack()
{
	/**
	 * Cases
	 * 1. Head = Tail -> Has one element
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
		Node* previous = get(size_ - 2);
		previous->next = nullptr;

		tail_->freeNode(nodeAllocator_);
		tail_ = previous;
	}

	--size_;
}


template <typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::clear()
{
	NodePtr node = head_;

	while (node)
	{
		NodePtr next = node->next;

		node->freeNode(nodeAllocator_);

		node = next;
	}

	size_ = 0;
	head_ = nullptr;
	tail_ = nullptr;
}


template <typename T, typename Allocator>
SingleLinkedList<T, Allocator>& SingleLinkedList<T, Allocator>::operator=(const SingleLinkedList& other)
{
	if (this == &other)
	{
		return *this;
	}

	copyFromAnother(other);

	return *this;
}


template <typename T, typename Allocator>
SingleLinkedList<T, Allocator>& SingleLinkedList<T, Allocator>::operator=(SingleLinkedList&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}

	moveFromAnother(std::move(other));

	return *this;
}


template <typename T, typename Allocator>
typename SingleLinkedList<T, Allocator>::Node* SingleLinkedList<T, Allocator>::get(size_t index)
{
	return const_cast<Node*>(static_cast<const SingleLinkedList&>(*this).get(index));
}


template <typename T, typename Allocator>
const typename SingleLinkedList<T, Allocator>::Node* SingleLinkedList<T, Allocator>::get(size_t index) const
{
	if (!isInBound(index))
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
void SingleLinkedList<T, Allocator>::remove(Iterator where)
{
	/**
	 * 1. empty
	 * 2. one elem
	 * 3. the usual
	 */

	assert(where != end());

	auto nodeToDelete = where.node_;
	if (nodeToDelete == head_)
	{
		popFront();
	}
	else
	{
		Node* previous = head_;
		while (previous && previous->next != nodeToDelete)
		{
			previous = previous->next;
		}

		if (previous && previous->next == nodeToDelete)
		{
			previous->next = nodeToDelete->next;
			if (nodeToDelete == tail_)
			{
				tail_ = previous;
			}

			nodeToDelete->freeNode(nodeAllocator_);
			--size_;
		}
	}
}

template <typename T, typename Allocator>
T& SingleLinkedList<T, Allocator>::front()
{
	return const_cast<T&>(static_cast<const SingleLinkedList&>(*this).front());
}


template <typename T, typename Allocator>
const T& SingleLinkedList<T, Allocator>::front() const
{
	assert(!isEmpty());
	return head_->value;
}


template <typename T, typename Allocator>
T& SingleLinkedList<T, Allocator>::back()
{
	assert(!isEmpty());
	return const_cast<T&>(static_cast<const SingleLinkedList&>(*this).back());
}


template <typename T, typename Allocator>
const T& SingleLinkedList<T, Allocator>::back() const
{
	assert(!isEmpty());
	return tail_->value;
}


template <typename T, typename Allocator>
typename SingleLinkedList<T, Allocator>::Iterator SingleLinkedList<T, Allocator>::begin()
{
	return Iterator( this, head_ );
}


template <typename T, typename Allocator>
typename SingleLinkedList<T, Allocator>::Iterator SingleLinkedList<T, Allocator>::end()
{
	// As I know, end iterators are not exactly the last item, but it's the item after the last.
	// So maybe this one is right...
	return Iterator( this, nullptr );
}


template <typename T, typename Allocator>
typename SingleLinkedList<T, Allocator>::ConstIterator SingleLinkedList<T, Allocator>::begin() const
{
	return ConstIterator( this, head_ );
}


template <typename T, typename Allocator>
typename SingleLinkedList<T, Allocator>::ConstIterator SingleLinkedList<T, Allocator>::end() const
{
	return ConstIterator( this, nullptr );
}


template <typename T, typename Allocator>
typename SingleLinkedList<T, Allocator>::ConstIterator SingleLinkedList<T, Allocator>::cbegin() const
{
	return begin();
}


template <typename T, typename Allocator>
typename SingleLinkedList<T, Allocator>::ConstIterator SingleLinkedList<T, Allocator>::cend() const
{
	return end();
}


template <typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::copyFromAnother(const SingleLinkedList<T>& other)
{
	if (!isEmpty())
	{
		clear();
	}

	NodePtr otherNode = other.head_;
	NodePtr previousNode = nullptr;
	while (otherNode)
	{
		NodePtr newNode = node_alloc_traits::allocate(nodeAllocator_, 1);
		node_alloc_traits::construct(nodeAllocator_, newNode, otherNode->value);

		if (head_ == nullptr)
		{
			head_ = newNode;
		}

		if (previousNode)
		{
			previousNode->next = newNode;
		}

		previousNode = newNode;
		otherNode = otherNode->next;
	}

	size_ = other.size_;
	tail_ = previousNode;
}


template <typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::moveFromAnother(SingleLinkedList<T>&& other)
{
	if (!isEmpty())
	{
		clear();
	}

	head_ = std::exchange(other.head_, nullptr);
	tail_ = std::exchange(other.tail_, nullptr);
	size_ = std::exchange(other.size_, 0);
}



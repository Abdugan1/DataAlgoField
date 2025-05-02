#pragma once
#include <algorithm>
#include <cassert>

template<typename ValType>
struct DoubleLinkedListNode final
{
	using Node = DoubleLinkedListNode<ValType>;
	using NodePtr = Node*;

	ValType value;
	NodePtr previous{ nullptr };
	NodePtr next{ nullptr };

	DoubleLinkedListNode() = default;

	DoubleLinkedListNode(const ValType& val, NodePtr previousNode, NodePtr nextNode)
		: value(val), previous(previousNode), next(nextNode)
	{
	}

	DoubleLinkedListNode(ValType&& val, NodePtr previousNode, NodePtr nextNode)
		: value(std::move(val)), previous(previousNode), next(nextNode)
	{
	}
};

template<typename T>
class DoubleLinkedList final
{
public:
	struct Node
	{
		T value{};
		Node* previous{ nullptr };
		Node* next{ nullptr };
	};

	class Iterator
	{
	public:
		Iterator(const DoubleLinkedList* owner, Node* node) : owner_{owner}, node_{node} {}

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
		const DoubleLinkedList* owner_ = nullptr;
		Node* node_{ nullptr };

		friend class DoubleLinkedList;
	};

	class ConstIterator
	{
	public:
		ConstIterator(const DoubleLinkedList* owner, Node* node) : owner_{ owner }, node_{ node } {}

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
		const DoubleLinkedList* owner_{ nullptr };
		Node* node_{ nullptr };
	};

public:
	DoubleLinkedList();
	DoubleLinkedList(std::initializer_list<T> vals);
	DoubleLinkedList(const DoubleLinkedList<T>& other);
	DoubleLinkedList(DoubleLinkedList<T>&& other) noexcept;
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

	DoubleLinkedList<T>& operator=(const DoubleLinkedList<T>& other);
	DoubleLinkedList<T>& operator=(DoubleLinkedList<T>&& other) noexcept;

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

	void copyFromAnother(const DoubleLinkedList<T>& other);
	void moveFromAnother(DoubleLinkedList<T>&& other);

	template<typename ValType>
	void insertAtBeginning(ValType&& val);

	template<typename ValType>
	void insertAtEnd(ValType&& val);

	Node* get(size_t index);
	const Node* get(size_t index) const;

private:
	Node* head_{ nullptr };
	Node* tail_{ nullptr };
	size_t size_{ 0 };
};

template <typename T>
constexpr size_t DoubleLinkedList<T>::size() const noexcept
{
	return size_;
}

template <typename T>
constexpr bool DoubleLinkedList<T>::isEmpty() const noexcept
{
	return size_ == 0;
}

template <typename T>
constexpr bool DoubleLinkedList<T>::isInBounds(size_t index) const noexcept
{
	return index < size_;
}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList()
{
}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList<T>& other)
{
	copyFromAnother(other);
}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(std::initializer_list<T> vals)
{
	size_ = vals.size();
	Node* previous = nullptr;
	for (auto&& val : vals)
	{
		Node* node = new Node;
		node->value = val;

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

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(DoubleLinkedList<T>&& other) noexcept
{
	moveFromAnother(std::move(other));
}

template <typename T>
DoubleLinkedList<T>::~DoubleLinkedList()
{
	clear();
}

template <typename T>
typename DoubleLinkedList<T>::Node* DoubleLinkedList<T>::get(size_t index)
{
	return const_cast<Node*>(static_cast<const DoubleLinkedList&>(*this).get(index));
}

template <typename T>
const typename DoubleLinkedList<T>::Node* DoubleLinkedList<T>::get(size_t index) const
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

template <typename T>
void DoubleLinkedList<T>::pushFront(const T& val)
{
	insertAtBeginning(val);
}

template <typename T>
void DoubleLinkedList<T>::pushFront(T&& val)
{
	insertAtBeginning(std::move(val));
}

template <typename T>
template <typename ValType>
void DoubleLinkedList<T>::insertAtBeginning(ValType&& val)
{
	/**
	 * 1. empty
	 * 2. one elem
	 * 3. the usual
	 */

	Node* newNode = new Node;
	newNode->value = std::forward<ValType>(val);

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


template <typename T>
void DoubleLinkedList<T>::pushBack(const T& val)
{
	insertAtEnd(val);
}

template <typename T>
void DoubleLinkedList<T>::pushBack(T&& val)
{
	insertAtEnd(std::move(val));
}

template <typename T>
template <typename ValType>
void DoubleLinkedList<T>::insertAtEnd(ValType&& val)
{
	/**
	 * 1. empty
	 * 2. one elem
	 * 3. the usual
	 */

	Node* newNode = new Node;
	newNode->value = std::forward<ValType>(val);

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

template <typename T>
void DoubleLinkedList<T>::popFront()
{
	/**
	 * 1. empty
	 * 2. one elem
	 * 3. the usual
	 */

	assert(!isEmpty());

	if (head_ == tail_)
	{
		delete head_;
		head_ = nullptr;
		tail_ = nullptr;
	}
	else
	{
		Node* next = head_->next;
		next->previous = nullptr;

		delete head_;
		head_ = next;
	}

	--size_;
}

template <typename T>
void DoubleLinkedList<T>::popBack()
{
	/**
	* 1. empty
	* 2. one elem
	* 3. the usual
	*/

	assert(!isEmpty());

	if (head_ == tail_)
	{
		delete head_;
		head_ = nullptr;
		tail_ = nullptr;
	}
	else
	{
		Node* previous = tail_->previous;
		previous->next = nullptr;

		delete tail_;
		tail_ = previous;
	}

	--size_;
}


template <typename T>
void DoubleLinkedList<T>::clear()
{
	Node* node = head_;
	while (node)
	{
		Node* next = node->next;
		delete node;
		node = next;
	}

	head_ = nullptr;
	tail_ = nullptr;
	size_ = 0;
}

template <typename T>
void DoubleLinkedList<T>::remove(Iterator where)
{
	/**
	 * 1. empty
	 * 2. one elem
	 * 3. beginning
	 * 4. end
	 * 5. 
	 */
	assert(where != end());


	Node* previousNode = where.node_->previous;
	Node* toDeleteNode = where.node_;
	Node* nextNode = where.node_->next;

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

		delete toDeleteNode;

		--size_;
	}
}

template <typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(const DoubleLinkedList<T>& other)
{
	if (this == &other)
	{
		return *this;
	}
	copyFromAnother(other);

	return *this;
}

template <typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(DoubleLinkedList<T>&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	moveFromAnother(std::move(other));

	return *this;
}

template <typename T>
T& DoubleLinkedList<T>::front()
{
	return const_cast<T&>(static_cast<const DoubleLinkedList&>(*this).front());
}

template <typename T>
const T& DoubleLinkedList<T>::front() const
{
	assert(!isEmpty());
	return head_->value;
}

template <typename T>
T& DoubleLinkedList<T>::back()
{
	return const_cast<T&>(static_cast<const DoubleLinkedList&>(*this).back());
}

template <typename T>
const T& DoubleLinkedList<T>::back() const
{
	assert(!isEmpty());
	return tail_->value;
}

template <typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::begin()
{
	return Iterator(this, head_);
}

template <typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::end()
{
	return Iterator(this, nullptr);
}

template <typename T>
typename DoubleLinkedList<T>::ConstIterator DoubleLinkedList<T>::begin() const
{
	return ConstIterator(this, head_);
}

template <typename T>
typename DoubleLinkedList<T>::ConstIterator DoubleLinkedList<T>::end() const
{
	return ConstIterator(this, nullptr);
}

template <typename T>
typename DoubleLinkedList<T>::ConstIterator DoubleLinkedList<T>::cbegin() const
{
	return begin();
}

template <typename T>
typename DoubleLinkedList<T>::ConstIterator DoubleLinkedList<T>::cend() const
{
	return end();
}

template <typename T>
void DoubleLinkedList<T>::copyFromAnother(const DoubleLinkedList<T>& other)
{
	if (!isEmpty())
	{
		clear();
	}

	Node* otherNode = other.head_;

	Node* previousNode = nullptr;
	while (otherNode)
	{
		Node* newNode = new Node;
		newNode->value = otherNode->value;

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

template <typename T>
void DoubleLinkedList<T>::moveFromAnother(DoubleLinkedList<T>&& other)
{
	if (!isEmpty())
	{
		clear();
	}

	head_ = std::exchange(other.head_, nullptr);
	tail_ = std::exchange(other.tail_, nullptr);
	size_ = std::exchange(other.size_, 0);
}

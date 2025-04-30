#pragma once
#include <algorithm>
#include <assert.h>
#include <initializer_list>

template<typename T>
class SingleLinkedList final
{
public:
	struct Node
	{
		T value;
		Node* next{ nullptr };
	};

	class Iterator
	{
	public:
		Iterator(const SingleLinkedList* owner, Node* node) : owner_{ owner }, node_{ node } {}

		T& operator*() { return node_->value; }
		const T& operator*() const { return node_->value; }

		Iterator& operator++() { node_ = node_->next; return *this; }
		Iterator operator++(int) { Iterator old( owner_, node_ ); node_ = node_->next; return old; }

		constexpr bool operator==(const Iterator& other) const
		{
			assert(owner_ == other.owner_);
			return node_ == other.node_;
		}
		constexpr bool operator!=(const Iterator& other) const { return !(*this == other); }

	private:
		const SingleLinkedList* owner_{ nullptr };
		Node* node_{nullptr};

		friend class SingleLinkedList;
	};

	class ConstIterator
	{
	public:
		ConstIterator(const SingleLinkedList* owner, Node* node) : owner_{ owner }, node_{ node } {}

		const T& operator*() const { return node_->value; }

		ConstIterator& operator++() { node_ = node_->next; return *this; }
		ConstIterator operator++(int) { ConstIterator old( owner_, node_ ); node_ = node_->next; return old; }

		constexpr bool operator==(const ConstIterator& other) const
		{
			assert(owner_ == other.owner_);
			return node_ == other.node_;
		}
		constexpr bool operator!=(const ConstIterator& other) const { return !(*this == other); }

	private:
		const SingleLinkedList* owner_{ nullptr };
		Node* node_{ nullptr };
	};

public:
	SingleLinkedList();
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

	Node* get(size_t index);
	const Node* get(size_t index) const;

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

	constexpr bool isInBound(size_t index) const noexcept;

private:
	Node* head_{ nullptr };
	Node* tail_{ nullptr };
	size_t size_{0};
};


template <typename T>
constexpr size_t SingleLinkedList<T>::size() const noexcept
{
	return size_;
}


template <typename T>
constexpr bool SingleLinkedList<T>::isEmpty() const noexcept
{
	return size_ == 0;
}


template <typename T>
constexpr bool SingleLinkedList<T>::isInBound(size_t index) const noexcept
{
	return index < size_;
}


template <typename T>
SingleLinkedList<T>::SingleLinkedList()
{
}


template <typename T>
SingleLinkedList<T>::SingleLinkedList(std::initializer_list<T> vals)
{
	size_ = vals.size();
	Node* previousNode = nullptr;

	for (auto&& val : vals)
	{
		Node* newNode = new Node;
		newNode->value = val;

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


template <typename T>
SingleLinkedList<T>::SingleLinkedList(const SingleLinkedList& other)
{
	copyFromAnother(other);
}


template <typename T>
SingleLinkedList<T>::SingleLinkedList(SingleLinkedList&& other) noexcept
{
	moveFromAnother(std::move(other));
}


template <typename T>
SingleLinkedList<T>::~SingleLinkedList()
{
	clear();
}


template <typename T>
void SingleLinkedList<T>::pushFront(const T& val)
{
	insertAtBeginning(val);
}


template <typename T>
void SingleLinkedList<T>::pushFront(T&& val)
{
	insertAtBeginning(std::move(val));
}


template <typename T>
template <typename ValType>
void SingleLinkedList<T>::insertAtBeginning(ValType&& val)
{
	Node* newNode = new Node;
	newNode->value = std::forward<ValType>(val);
	newNode->next = head_;
	head_ = newNode;

	if (size_ == 0)
	{
		tail_ = head_;
	}

	++size_;
}


template <typename T>
void SingleLinkedList<T>::pushBack(const T& val)
{
	insertAtEnd(val);
}


template <typename T>
void SingleLinkedList<T>::pushBack(T&& val)
{
	insertAtEnd(val);
}


template <typename T>
void SingleLinkedList<T>::popFront()
{
	/**
	 * 1. empty
	 * 2. one element
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
		Node* toDelete = head_;
		head_ = head_->next;
		delete toDelete;
	}

	--size_;
}


template <typename T>
template <typename ValType>
void SingleLinkedList<T>::insertAtEnd(ValType&& val)
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


template <typename T>
void SingleLinkedList<T>::popBack()
{
	/**
	 * Cases
	 * 1. Head = Tail -> Has one element
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
		Node* previous = get(size_ - 2);
		previous->next = nullptr;

		delete tail_;
		tail_ = previous;
	}

	--size_;
}


template <typename T>
void SingleLinkedList<T>::clear()
{
	Node* node = head_;

	while (node)
	{
		Node* next = node->next;
		delete node;
		node = next;
	}

	size_ = 0;
	head_ = nullptr;
	tail_ = nullptr;
}


template <typename T>
SingleLinkedList<T>& SingleLinkedList<T>::operator=(const SingleLinkedList& other)
{
	if (this == &other)
	{
		return *this;
	}

	copyFromAnother(other);

	return *this;
}


template <typename T>
SingleLinkedList<T>& SingleLinkedList<T>::operator=(SingleLinkedList&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}

	moveFromAnother(std::move(other));

	return *this;
}


template <typename T>
typename SingleLinkedList<T>::Node* SingleLinkedList<T>::get(size_t index)
{
	return const_cast<Node*>(static_cast<const SingleLinkedList&>(*this).get(index));
}


template <typename T>
const typename SingleLinkedList<T>::Node* SingleLinkedList<T>::get(size_t index) const
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

template <typename T>
void SingleLinkedList<T>::remove(Iterator where)
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

			delete nodeToDelete;
			--size_;
		}
	}
}

template <typename T>
T& SingleLinkedList<T>::front()
{
	return const_cast<T&>(static_cast<const SingleLinkedList&>(*this).front());
}


template <typename T>
const T& SingleLinkedList<T>::front() const
{
	assert(!isEmpty());
	return head_->value;
}


template <typename T>
T& SingleLinkedList<T>::back()
{
	assert(!isEmpty());
	return const_cast<T&>(static_cast<const SingleLinkedList&>(*this).back());
}


template <typename T>
const T& SingleLinkedList<T>::back() const
{
	assert(!isEmpty());
	return tail_->value;
}

template <typename T>
typename SingleLinkedList<T>::Iterator SingleLinkedList<T>::begin()
{
	return Iterator( this, head_ );
}

template <typename T>
typename SingleLinkedList<T>::Iterator SingleLinkedList<T>::end()
{
	// As I know, end iterators are not exactly the last item, but it's the item after the last.
	// So maybe this one is right...
	return Iterator( this, nullptr );
}

template <typename T>
typename SingleLinkedList<T>::ConstIterator SingleLinkedList<T>::begin() const
{
	return ConstIterator( this, head_ );
}

template <typename T>
typename SingleLinkedList<T>::ConstIterator SingleLinkedList<T>::end() const
{
	return ConstIterator( this, nullptr );
}

template <typename T>
typename SingleLinkedList<T>::ConstIterator SingleLinkedList<T>::cbegin() const
{
	return begin();
}

template <typename T>
typename SingleLinkedList<T>::ConstIterator SingleLinkedList<T>::cend() const
{
	return end();
}

template <typename T>
void SingleLinkedList<T>::copyFromAnother(const SingleLinkedList<T>& other)
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
		}

		previousNode = newNode;
		otherNode = otherNode->next;
	}

	size_ = other.size_;
	tail_ = previousNode;
}


template <typename T>
void SingleLinkedList<T>::moveFromAnother(SingleLinkedList<T>&& other)
{
	if (!isEmpty())
	{
		clear();
	}

	head_ = std::exchange(other.head_, nullptr);
	tail_ = std::exchange(other.tail_, nullptr);
	size_ = std::exchange(other.size_, 0);
}



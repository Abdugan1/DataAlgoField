#pragma once
#include <memory>

#include "SingleLinkedList.h"

template<
	typename T, 
	template<typename> typename Container = SingleLinkedList
>
class Queue final
{
public:
	using size_type = size_t;
	using reference = T&;
	using const_reference = const T&;
public:
	Queue() = default;

	Queue(const Queue&) = default;
	Queue(Queue&&) noexcept = default;
	~Queue() = default;

	Queue& operator=(const Queue&) = default;
	Queue& operator=(Queue&&) noexcept = default;

	void push(const T& val);
	void push(T&& val);

	void pop();

	void clear();

	[[nodiscard]] reference front();
	[[nodiscard]] const_reference front() const;

	[[nodiscard]] bool isEmpty() const noexcept(noexcept(container_.isEmpty()));
	[[nodiscard]] size_type size() const noexcept(noexcept(container_.size()));

private:
	Container<T> container_;
};


template<typename T, template<typename> typename Container>
void Queue<T, Container>::push(const T& val)
{
	container_.pushBack(val);
}


template<typename T, template<typename> typename Container>
void Queue<T, Container>::push(T&& val)
{
	container_.pushBack(std::move(val));
}

template<typename T, template<typename> typename Container>
void Queue<T, Container>::pop()
{
	container_.popFront();
}

template<typename T, template<typename> typename Container>
void Queue<T, Container>::clear()
{
	container_.clear();
}

template<typename T, template<typename> typename Container>
Queue<T, Container>::reference Queue<T, Container>::front()
{
	return container_.front();
}

template<typename T, template<typename> typename Container>
Queue<T, Container>::const_reference Queue<T, Container>::front() const
{
	return container_.front();
}

template<typename T, template<typename> typename Container>
bool Queue<T, Container>::isEmpty() const noexcept(noexcept(container_.isEmpty()))
{
	return container_.isEmpty();
}

template<typename T, template<typename> typename Container>
Queue<T, Container>::size_type Queue<T, Container>::size() const noexcept(noexcept(container_.size()))
{
	return container_.size();
}

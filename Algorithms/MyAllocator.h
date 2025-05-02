#pragma once
#include <utility>

template<typename T>
struct MyAllocator
{
	T* allocate(size_t n)
	{
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}

	void deallocate(T* p, size_t)
	{
		::operator delete(p);
	}

	template <typename... Args>
	void construct(T* p, Args&&... args)
	{
		new (p)T(std::forward<Args>(args)...);
	}

	void destroy(T* p)
	{
		p->~T();
	}
};

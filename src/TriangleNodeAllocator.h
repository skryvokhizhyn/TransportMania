#pragma once

#include "TriangleNodePool.h"
#include <cstddef>

namespace trm
{
namespace terrain
{
namespace lod
{
	// the minimum required for std::allocate_shared method 
	//to construct shared_ptr from TriangleNodePool
	template<typename T>
	class TriangleNodeAllocator
	{
	public:
		template<typename U>
		struct rebind
		{
			using other = TriangleNodeAllocator<U>;
		};

	public:
		TriangleNodeAllocator()
		{}

		template<typename U>
		TriangleNodeAllocator(const TriangleNodeAllocator<U> &)
		{}

		T * allocate(const std::size_t n);
		void deallocate(T * p, const std::size_t n);
		void destroy(const T * ptr);
	};

	template<typename T>
	T *
	TriangleNodeAllocator<T>::allocate(const std::size_t n)
	{
		assert(n == 1);(n);

		void * p = TriangleNodePool::Get<T>().Malloc();
		if (p == nullptr)
		{
			throw std::bad_alloc();
		}

		return static_cast<T*>(p);
	}

	template<typename T>
	void
	TriangleNodeAllocator<T>::deallocate(T * p, const std::size_t n)
	{
		assert(n == 1);(n);

		TriangleNodePool::Get<T>().Free(p);
	}
	
	// stolen from boost implementation
	template<typename T>
	void 
	TriangleNodeAllocator<T>::destroy(const T * ptr)
    { //! Destroy ptr using destructor.
		ptr->~T();
		(void) ptr; // Avoid unused variable warning.
    }
} // namespace lod
} // namespace terrain
} // namespace trm
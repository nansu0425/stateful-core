// lib-stateful-core/memory/Allocator.hpp

#pragma once

namespace StatefulCore
{
	namespace Memory
	{
		/*----------------------*
		 *    StompAllocator    *
		 *----------------------*/

		class StompAllocator
		{
		private:
			enum { PAGE_SIZE = 0x1000 };

		public:
			static void*    Allocate(int32 size);
			static void	    Release(void* ptr);
		};

		/*---------------------*
		 *    PoolAllocator    *
		 *---------------------*/

		class PoolAllocator
		{
		public:
			static void*    Allocate(int32 size);
			static void     Release(void* ptr);
		};

		/*--------------------*
		 *    StlAllocator    *
		 *--------------------*/

		template<typename T>
		class StlAllocator
		{
		public:
			using value_type = T;

			StlAllocator() {}

			template<typename Other>
			StlAllocator(const StlAllocator<Other>&) {}

			T* allocate(size_t count)
			{
				const int32 size = static_cast<int32>(count * sizeof(T));

				return static_cast<T*>(PoolAllocator::Allocate(size));
			}

			void deallocate(T* ptr, size_t count)
			{
				PoolAllocator::Release(ptr);
			}
		};
	}
}

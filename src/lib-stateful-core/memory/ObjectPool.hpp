// lib-stateful-core/memory/ObjectPool.hpp

#pragma once

#include <lib-stateful-core/memory/MemoryPool.hpp>

namespace StatefulCore
{
	namespace Memory
	{
		/*------------------*
		 *    ObjectPool    *
		 *------------------*/

		template<typename T>
		class ObjectPool
		{
		public:
			template<typename... Args>
			static T* Pop(Args&&... args)
			{
#ifdef _STOMP
				MemoryHeader* header =
					reinterpret_cast<MemoryHeader*>(StompAllocator::Allocate(s_allocSize));
				T* memory = static_cast<T*>(
					MemoryHeader::AttachHeader(header, s_allocSize)
					);
#else
				T* memory = static_cast<T*>(
					MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize)
					);
#endif // _STOMP

				new(memory) T(std::forward<Args>(args)...);

				return memory;
			}

			static void Push(T* obj)
			{
				obj->~T();

#ifdef _STOMP
				StompAllocator::Release(MemoryHeader::DetachHeader(obj));
#else
				s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif // _STOMP
			}

			template <typename... Args>
			static SPtr<T> MakeShared(Args&&... args)
			{
				SPtr<T> sPtr(Pop(std::forward<Args>(args)...), Push);

				return sPtr;
			}

		private:
			static int32         s_allocSize;
			static MemoryPool    s_pool;
		};

		template<typename T>
		int32 ObjectPool<T>::s_allocSize = sizeof(T) + sizeof(MemoryHeader);

		template<typename T>
		MemoryPool ObjectPool<T>::s_pool(s_allocSize);
	}
}

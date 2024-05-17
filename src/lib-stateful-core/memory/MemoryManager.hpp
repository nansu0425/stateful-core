// lib-stateful-core/memory/MemoryManager.hpp

#pragma once

namespace StatefulCore
{
	namespace Memory
	{
		class MemoryPool;

		/*---------------------*
		 *    MemoryManager    *
		 *---------------------*/

		class MemoryManager
		{
		private:
			enum
			{
				// 1~1024 (32)     : 32, 64, 96, ... , 1024 
				// 1025~2048 (128) : 1152, 1280, 1408, ... , 2048
				// 2049~4048 (256) : 2304, 2560, 2816, ... , 4096
				POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
				MAX_ALLOC_SIZE = 4096,
			};

		public:
			MemoryManager();
			~MemoryManager();

		public:
			void*    Allocate(int32 size);
			void     Release(void* ptr);

		private:
			Vec<MemoryPool*>    m_pools;
			MemoryPool*         m_poolTable[MAX_ALLOC_SIZE + 1];
		};

		/*-----------------*
		 *    Templates    *
		 *-----------------*/

		template<typename Type, typename... Args>
		Type* New(Args&&... args)
		{
			Type* memory = static_cast<Type*>(PoolAllocator::Allocate(sizeof(Type)));
			new(memory) Type(std::forward<Args>(args)...);

			return memory;
		}

		template<typename Type>
		void Delete(Type* obj)
		{
			obj->~Type();
			PoolAllocator::Release(obj);
		}

		template<typename Type, typename... Args>
		SPtr<Type> MakeShared(Args&&... args)
		{
			SPtr<Type> sPtr(New<Type>(std::forward<Args>(args)...), Delete<Type>);

			return sPtr;
		}
	}
}

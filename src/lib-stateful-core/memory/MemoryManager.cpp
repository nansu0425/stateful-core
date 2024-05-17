// lib-stateful-core/memory/MemoryManager.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/memory/MemoryManager.hpp>
#include <lib-stateful-core/memory/MemoryPool.hpp>

namespace StatefulCore
{
	namespace Memory
	{
		MemoryManager::MemoryManager()
		{
			int32 size = 0;
			int32 tableIdx = 1;

			m_poolTable[0] = nullptr;

			for (size = 32; size <= 1024; size += 32)
			{
				MemoryPool* pool = new MemoryPool(size);
				m_pools.push_back(pool);

				while (tableIdx <= size)
					m_poolTable[tableIdx++] = pool;
			}

			for (size = 1024 + 128; size <= 2048; size += 128)
			{
				MemoryPool* pool = new MemoryPool(size);
				m_pools.push_back(pool);

				while (tableIdx <= size)
					m_poolTable[tableIdx++] = pool;
			}

			for (size = 2048 + 256; size <= 4096; size += 256)
			{
				MemoryPool* pool = new MemoryPool(size);
				m_pools.push_back(pool);

				while (tableIdx <= size)
					m_poolTable[tableIdx++] = pool;
			}
		}
		MemoryManager::~MemoryManager()
		{
			for (MemoryPool* pool : m_pools)
				delete pool;

			m_pools.clear();
		}
		void* MemoryManager::Allocate(int32 size)
		{
			MemoryHeader* header = nullptr;
			const int32 allocSize = size + sizeof(MemoryHeader);
			assert(allocSize > 0);

#ifdef _STOMP
			header = reinterpret_cast<MemoryHeader*>(StompAllocator::Allocate(allocSize));
#else
			if (allocSize > MAX_ALLOC_SIZE)
			{
				header = reinterpret_cast<MemoryHeader*>(
					::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
			}
			else
			{
				header = m_poolTable[allocSize]->Pop();
			}
#endif // _STOMP

			return MemoryHeader::AttachHeader(header, allocSize);
		}

		void MemoryManager::Release(void* ptr)
		{
			MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

			const int32 allocSize = header->allocSize;
			assert(allocSize > 0);

#ifdef _STOMP
			StompAllocator::Release(header);
#else
			if (allocSize > MAX_ALLOC_SIZE)
			{
				::_aligned_free(header);
			}
			else
			{
				m_poolTable[allocSize]->Push(header);
			}
#endif // _STOMP
		}
	}
}

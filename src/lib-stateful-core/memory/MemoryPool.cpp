// lib-stateful-core/memory/MemoryPool.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/memory/MemoryPool.hpp>

namespace StatefulCore
{
	namespace Memory
	{
		MemoryPool::MemoryPool(int32 allocSize) : m_allocSize(allocSize)
		{
			::InitializeSListHead(&m_sListHeader);
		}

		MemoryPool::~MemoryPool()
		{
			while (MemoryHeader* memory = 
				static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&m_sListHeader)))
			{
				::_aligned_free(memory);
			}
		}

		void MemoryPool::Push(MemoryHeader* ptr)
		{
			ptr->allocSize = 0;
			::InterlockedPushEntrySList(&m_sListHeader, static_cast<PSLIST_ENTRY>(ptr));

			m_usedCount.fetch_sub(1);
			m_reservedCount.fetch_add(1);
		}

		MemoryHeader* MemoryPool::Pop()
		{
			MemoryHeader* memory =
				static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&m_sListHeader));

			if (memory == nullptr)
			{
				memory = reinterpret_cast<MemoryHeader*>(
					::_aligned_malloc(m_allocSize, SLIST_ALIGNMENT));
			}
			else
			{
				assert(memory->allocSize == 0);
				m_reservedCount.fetch_sub(1);
			}

			m_usedCount.fetch_add(1);

			return memory;
		}
	}
}

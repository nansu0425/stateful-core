// lib-stateful-core/memory/Allocator.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/memory/Allocator.hpp>

namespace StatefulCore
{
	namespace Memory
	{
		void* StompAllocator::Allocate(int32 size)
		{
			const int64 pageCnt = (size + PAGE_SIZE - 1) / PAGE_SIZE;
			const int64 offset = pageCnt * PAGE_SIZE - size;

			void* baseAddr = ::VirtualAlloc(
				NULL, pageCnt * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE
			);

			return static_cast<void*>(static_cast<BYTE*>(baseAddr) + offset);
		}

		void StompAllocator::Release(void* ptr)
		{
			const int64 addr = reinterpret_cast<int64>(ptr);
			const int64 baseAddr = addr - (addr % PAGE_SIZE);

			::VirtualFree(reinterpret_cast<void*>(baseAddr), 0, MEM_RELEASE);
		}
	}
}

// lib-stateful-core/memory/MemoryPool.hpp>

#pragma once

namespace StatefulCore
{
	namespace Memory
	{
		enum
		{
			SLIST_ALIGNMENT = 16
		};

		/*--------------------*
		 *    MemoryHeader    *
		 *--------------------*/

		DECLSPEC_ALIGN(SLIST_ALIGNMENT)
		struct MemoryHeader : public SLIST_ENTRY
		{
			MemoryHeader(int32 size) :allocSize(size) { }

			// header -> [MemoryHeader]
			//           [data]
			// return pointer to [data] 
			static void* AttachHeader(MemoryHeader* header, int32 size)
			{
				new(header) MemoryHeader(size);

				return reinterpret_cast<void*>(++header);
			}

			//        [MemoryHeader]
			// ptr -> [data]
			// return pointer to [MemoryHeader]
			static MemoryHeader* DetachHeader(void* ptr)
			{
				MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;

				return header;
			}

			int32 allocSize;
		};

		/*------------------*
		 *    MemoryPool    *
		 *------------------*/

		DECLSPEC_ALIGN(SLIST_ALIGNMENT)
		class MemoryPool
		{
		public:
			MemoryPool(int32 allocSize);
			~MemoryPool();
		
		public:
			void             Push(MemoryHeader* ptr);
			MemoryHeader*    Pop();

		private:
			SLIST_HEADER     m_sListHeader;
			int32            m_allocSize = 0;
			Atomic<int32>    m_usedCount = 0;
			Atomic<int32>    m_reservedCount = 0;
		};
	}
}

// lib-stateful-core/memory/Allocator.hpp

#pragma once

namespace StatefulCore
{
	namespace Memory
	{
		class StompAllocator
		{
		private:
			enum { PAGE_SIZE = 0x1000 };

		public:
			static void*    Allocate(int32 size);
			static void	    Release(void* ptr);
		};
	}
}

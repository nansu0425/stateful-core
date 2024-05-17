// lib-stateful-core/common/Global.hpp

#pragma once

namespace StatefulCore
{
	namespace Multithreading
	{
		extern class ThreadManager*       g_threadManager;
		extern class DeadLockProfiler*    g_deadLockProfiler;
	}
	
	namespace Memory
	{
		extern class MemoryManager*    g_memoryManager;
	}

	class Global
	{
	public:
		Global();
		~Global();
	};

	extern Global g_global;
}

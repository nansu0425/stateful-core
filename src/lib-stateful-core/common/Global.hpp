// lib-stateful-core/common/Global.hpp

#pragma once

namespace StatefulCore
{
	namespace Multithreading
	{
		extern class ThreadManager*        g_threadManager;
		extern class DeadLockProfiler*     g_deadLockProfiler;
	}
	
	namespace Memory
	{
		extern class MemoryManager*        g_memoryManager;
	}

	namespace Network
	{
		extern class SendBufferManager*    g_sendBufferManager;
	}

	namespace Asynchronous
	{
		extern class JobQueueManager*      g_jobQueueManager;
		extern class JobTimer*             g_jobTimer;
	}

	class Global
	{
	public:
		Global();
		~Global();
	};

	extern Global g_global;
}

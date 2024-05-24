// lib-stateful-core/common/ThreadLocalStorage.hpp

#pragma once

namespace StatefulCore
{
	namespace Multithreading
	{
		extern thread_local ThreadId              l_threadId;
		extern thread_local std::stack<LockId>    l_lockStack;
	}

	namespace Network
	{
		class SendBuffer;

		extern thread_local SPtr<SendBuffer>    l_sendBuf;
	}

	namespace Asynchronous
	{
		class JobQueue;

		extern thread_local SPtr<JobQueue>    l_curJobQueue;
	}
}

// lib-stateful-core/common/ThreadLocalStorage.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/common/ThreadLocalStorage.hpp>

namespace StatefulCore
{
	namespace Multithreading
	{
		thread_local ThreadId              l_threadId = 0;
		thread_local std::stack<LockId>    l_lockStack;
	}

	namespace Network
	{
		thread_local SPtr<SendBuffer>    l_sendBuf = nullptr;
	}
}


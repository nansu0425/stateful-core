// lib-stateful-core/common/ThreadLocalStorage.hpp

#pragma once

namespace StatefulCore
{
	extern thread_local ThreadId              l_threadId;
	extern thread_local std::stack<LockId>    l_lockStack;
}

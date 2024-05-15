// lib-stateful-core/common/ThreadLocalStorage.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/common/ThreadLocalStorage.hpp>

namespace StatefulCore
{
	thread_local ThreadId    l_threadId = 0;
}


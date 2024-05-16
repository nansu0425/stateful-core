// lib-stateful-core/common/Global.hpp

#pragma once

#include <lib-stateful-core/multithreading/ThreadManager.hpp>
#include <lib-stateful-core/multithreading/DeadLockProfiler.hpp>

namespace StatefulCore
{
	extern Multithreading::ThreadManager*       g_threadManager;
	extern Multithreading::DeadLockProfiler*    g_deadLockProfiler;

	class Global
	{
	public:
		Global();
		~Global();
	};

	extern Global g_global;
}

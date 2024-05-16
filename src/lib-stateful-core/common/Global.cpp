// lib-stateful-core/common/Global.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/common/Global.hpp>

namespace StatefulCore
{
	Multithreading::ThreadManager*       g_threadManager = nullptr;
	Multithreading::DeadLockProfiler*    g_deadLockProfiler = nullptr;
	
	Global::Global()
	{
		g_threadManager = new Multithreading::ThreadManager();
		g_deadLockProfiler = new Multithreading::DeadLockProfiler();
	}

	Global::~Global()
	{
		delete g_threadManager;
		delete g_deadLockProfiler;
	}

	Global g_global;
}

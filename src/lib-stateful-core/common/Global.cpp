// lib-stateful-core/common/Global.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/common/Global.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>
#include <lib-stateful-core/multithreading/DeadLockProfiler.hpp>
#include <lib-stateful-core/memory/MemoryManager.hpp>

namespace StatefulCore
{
	namespace Multithreading
	{
		ThreadManager* g_threadManager;
		DeadLockProfiler* g_deadLockProfiler;
	}

	namespace Memory
	{
		MemoryManager* g_memoryManager;
	}
	
	Global::Global()
	{
		Multithreading::g_threadManager = new Multithreading::ThreadManager();
		Multithreading::g_deadLockProfiler = new Multithreading::DeadLockProfiler();
		Memory::g_memoryManager = new Memory::MemoryManager();
	}

	Global::~Global()
	{
		delete Multithreading::g_threadManager;
		delete Multithreading::g_deadLockProfiler;
		delete Memory::g_memoryManager;
	}

	Global g_global;
}

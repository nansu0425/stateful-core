// lib-stateful-core/common/Global.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/common/Global.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>
#include <lib-stateful-core/multithreading/DeadLockProfiler.hpp>
#include <lib-stateful-core/memory/MemoryManager.hpp>
#include <lib-stateful-core/network/SendBufferManager.hpp>
#include <lib-stateful-core/network/SocketFunctions.hpp>

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

	namespace Network
	{
		SendBufferManager* g_sendBufferManager;
	}
	
	Global::Global()
	{
		Multithreading::g_threadManager = new Multithreading::ThreadManager();
		Multithreading::g_deadLockProfiler = new Multithreading::DeadLockProfiler();
		Memory::g_memoryManager = new Memory::MemoryManager();
		Network::g_sendBufferManager = new Network::SendBufferManager();
		Network::Init();
	}

	Global::~Global()
	{
		delete Multithreading::g_threadManager;
		delete Multithreading::g_deadLockProfiler;
		delete Memory::g_memoryManager;
		delete Network::g_sendBufferManager;
		Network::Clear();
	}

	Global g_global;
}

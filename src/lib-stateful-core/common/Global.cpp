// lib-stateful-core/common/Global.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/common/Global.hpp>

namespace StatefulCore
{
	Multithreading::ThreadManager* g_threadManager = nullptr;
	
	Global::Global()
	{
		g_threadManager = new Multithreading::ThreadManager();
	}

	Global::~Global()
	{
		delete g_threadManager;
	}

	Global g_global;
}

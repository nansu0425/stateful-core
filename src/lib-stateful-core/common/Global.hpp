// lib-stateful-core/common/Global.hpp

#pragma once

#include <lib-stateful-core/multithreading/ThreadManager.hpp>

namespace StatefulCore
{
	extern Multithreading::ThreadManager* g_threadManager;

	class Global
	{
	public:
		Global();
		~Global();
	};

	extern Global g_global;
}

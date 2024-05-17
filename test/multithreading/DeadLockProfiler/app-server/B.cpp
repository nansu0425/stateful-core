#include "app-server/precompiled/Pch.hpp"
#include "B.hpp"

B g_b;

void B::LockA()
{
	W_SPIN_LOCK;
	std::cout << StatefulCore::l_threadId << ": Acquire B" << std::endl;
	g_a.LockSelf();
	std::cout << StatefulCore::l_threadId << ": Release A" << std::endl;
}

void B::LockC()
{
	W_SPIN_LOCK;
	std::cout << StatefulCore::l_threadId << ": Acquire B" << std::endl;
	g_c.LockSelf();
	std::cout << StatefulCore::l_threadId << ": Release C" << std::endl;
}

void B::LockSelf()
{
	W_SPIN_LOCK;
	std::cout << StatefulCore::l_threadId << ": Acquire B" << std::endl;
}

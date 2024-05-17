#include "app-server/precompiled/Pch.hpp"
#include "C.hpp"

C g_c;

void C::LockA()
{
	W_SPIN_LOCK;
	std::cout << StatefulCore::l_threadId << ": Acquire C" << std::endl;
	g_a.LockSelf();
	std::cout << StatefulCore::l_threadId << ": Release A" << std::endl;
}

void C::LockB()
{
	W_SPIN_LOCK;
	std::cout << StatefulCore::l_threadId << ": Acquire C" << std::endl;
	g_b.LockSelf();
	std::cout << StatefulCore::l_threadId << ": Release B" << std::endl;
}

void C::LockSelf()
{
	W_SPIN_LOCK;
	std::cout << StatefulCore::l_threadId << ": Acquire C" << std::endl;
}

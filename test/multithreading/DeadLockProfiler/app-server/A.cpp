#include "app-server/precompiled/Pch.hpp"
#include "A.hpp"

A g_a;

void A::LockB()
{
	W_SPIN_LOCK;
	std::cout << StatefulCore::l_threadId << ": Acquire A" << std::endl;
	g_b.LockSelf();
	std::cout << StatefulCore::l_threadId << ": Release B" << std::endl;
}

void A::LockC()
{
	W_SPIN_LOCK;
	std::cout << StatefulCore::l_threadId << ": Acquire A" << std::endl;
	g_c.LockSelf();
	std::cout << StatefulCore::l_threadId << ": Release C" << std::endl;
}

void A::LockSelf()
{
	W_SPIN_LOCK;
	std::cout << StatefulCore::l_threadId << ": Acquire A" << std::endl;
}

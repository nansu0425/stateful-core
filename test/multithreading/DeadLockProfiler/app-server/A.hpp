#pragma once

#include <app-server/B.hpp>
#include <app-server/C.hpp>

class A
{
public:
	void LockB();
	void LockC();
	void LockSelf();

private:
	USE_RW_SPIN_LOCK;
};

extern A g_a;

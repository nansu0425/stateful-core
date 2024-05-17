#pragma once

#include <app-server/A.hpp>
#include <app-server/C.hpp>

class B
{
public:
	void LockA();
	void LockC();
	void LockSelf();

private:
	USE_RW_SPIN_LOCK;
};

extern B g_b;

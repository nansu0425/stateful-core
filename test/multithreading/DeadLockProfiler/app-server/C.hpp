#pragma once

#include <app-server/A.hpp>
#include <app-server/B.hpp>

class C
{
public:
	void LockA();
	void LockB();
	void LockSelf();

private:
	USE_RW_SPIN_LOCK;
};

extern C g_c;

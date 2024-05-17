// app-server/main/Main.cpp

#include <app-server/precompiled/Pch.hpp>

using namespace StatefulCore;

class Base
{
public:
	void SetBaseM1(int64 val) { m_baseM1 = val; }
	void SetBaseM2(int64 val) { m_baseM2 = val; }

private:
	int64 m_baseM1 = 0;
	int64 m_baseM2 = 0;
};

class Derived : public Base
{
public:
	void SetDerivedM(int64 val) { m_derivedM = val; }

private:
	int64 m_derivedM = 0;
};

int main()
{
	std::cout << "server started..." << std::endl;

	// Detect overflow
	Derived* derived = (Derived*)Memory::New<Base>();
	derived->SetBaseM1(0xa);
	derived->SetBaseM2(0xb);
	derived->SetDerivedM(0xc);

	// Detect use-after-free
	Derived* derived = Memory::New<Derived>();
	derived->SetBaseM1(1);
	derived->SetBaseM2(2);
	Memory::Delete(derived);
	derived->SetDerivedM(3);

	return 0;
}

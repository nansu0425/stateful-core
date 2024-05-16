// app-server/main/Main.cpp

#include <app-server/precompiled/Pch.hpp>
#include <lib-stateful-core/multithreading/RwLock.hpp>

using namespace StatefulCore;

class Test
{
public:
	void IncreaseShared()
	{
		W_SPIN_LOCK;
		std::cout << l_threadId << ": " << ++m_shared << std::endl;
	}

	void DecreaseShared()
	{
		W_SPIN_LOCK;
		std::cout << l_threadId << ": " << --m_shared << std::endl;
	}

	void PrintShared()
	{
		std::cout << m_shared << std::endl;
	}

private:
	USE_RW_SPIN_LOCK;
	int32 m_shared = 0;
};

Test g_test;

int main()
{
	std::cout << "server started..." << std::endl;

	for (uint32 i = 0; i < Thread::hardware_concurrency() / 2; i++)
	{
		g_threadManager->Launch([=]()
			{
				for (int32 i = 0; i < 100; i++)
				{
					g_test.IncreaseShared();
				}
			});

		g_threadManager->Launch([=]()
			{
				for (int32 i = 0; i < 100; i++)
				{
					g_test.DecreaseShared();
				}
			});
	}

	g_threadManager->Join();

	g_test.PrintShared();

	return 0;
}

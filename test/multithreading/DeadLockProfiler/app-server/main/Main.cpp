// app-server/main/Main.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/A.hpp>
#include <app-server/B.hpp>
#include <app-server/C.hpp>

using namespace StatefulCore;

int main()
{
	std::cout << "server started..." << std::endl;

	g_threadManager->Launch([=]()
		{
			while (true)
			{
				std::cout << l_threadId << ": A -> B" << std::endl;
				g_a.LockB();
				std::cout << l_threadId << ": Release A" << std::endl;
				std::this_thread::sleep_for(10ms);
			}
		});

	g_threadManager->Launch([=]()
		{
			while (true)
			{
				std::cout << l_threadId << ": B -> C" << std::endl;
				g_b.LockC();
				std::cout << l_threadId << ": Release B" << std::endl;
				std::this_thread::sleep_for(10ms);
			}
		});

	g_threadManager->Launch([=]()
		{
			while (true)
			{
				std::cout << l_threadId << ": C -> A" << std::endl;
				g_c.LockA();
				std::cout << l_threadId << ": Release C" << std::endl;
				std::this_thread::sleep_for(10ms);
			}
		});

	g_threadManager->Join();

	return 0;
}

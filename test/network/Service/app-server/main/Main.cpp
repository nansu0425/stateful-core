// app-server/main/Main.cpp

#include <app-server/precompiled/Pch.hpp>
#include <lib-stateful-core/network/Service.hpp>
#include <app-server/network/Sessions.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>

int main()
{
	std::cout << "Server started..." << std::endl;

	SPtr<Network::ServerService> service = Memory::MakeShared<Network::ServerService>(
		Network::SockaddrWrapper(L"127.0.0.1", 7777),
		Memory::MakeShared<Server::Network::ClientSession>,
		Memory::MakeShared<Network::IocpEventForwarder>(), 
		100);

	assert(service->Launch());

	for (int32 i = 0; i < 4; i++)
	{
		Multithreading::g_threadManager->Launch([=]()
			{
				while (true)
					service->GetIocpEventForwarder()->ForwardEvent2Handler();
			});
	}

	Multithreading::g_threadManager->Join();

	return 0;
}

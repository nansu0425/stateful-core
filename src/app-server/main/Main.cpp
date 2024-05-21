// app-server/main/Main.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/network/Sessions.hpp>
#include <lib-stateful-core/network/Service.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>

int main()
{
	SPtr<Network::ServerService> service = Memory::MakeShared<Network::ServerService>(
		Network::SockaddrWrapper(L"127.0.0.1", 7777),
		Memory::MakeShared<Server::Network::ClientSession>,
		Memory::MakeShared<Network::IocpEventForwarder>());

	assert(service->Launch());
	std::cout << "Launched server service" << std::endl;

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

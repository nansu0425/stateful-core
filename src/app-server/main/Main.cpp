// app-server/main/Main.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/network/Sessions.hpp>
#include <app-server/network/PacketHandler.hpp>
#include <lib-stateful-core/network/Service.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>

int main()
{
	SPtr<Network::ServerService> service = Memory::MakeShared<Network::ServerService>(
		Network::SockaddrWrapper(L"127.0.0.1", 7720),
		Memory::MakeShared<Server::Network::ClientSession>,
		Memory::MakeShared<Network::IocpEventForwarder>(),
		100);

	assert(service->Launch());
	std::cout << "Launched server service" << std::endl;

	Server::Network::PacketHandler::Init();
	std::cout << "Initialized packet handler" << std::endl;

	for (int32 i = 0; i < 4; i++)
	{
		Multithreading::g_threadManager->Launch([=]()
			{
				while (true)
					service->GetIocpEventForwarder()->ForwardEvent2Handler();
			});
	}
	std::cout << "Launched ForwardEvent2Handler() loop threads" << std::endl;

	Multithreading::g_threadManager->Join();

	return 0;
}

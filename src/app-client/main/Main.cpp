// app-client/main/Main.cpp

#include <app-client/precompiled/Pch.hpp>
#include <app-client/network/Sessions.hpp>
#include <lib-stateful-core/network/Service.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>

int main()
{
	std::this_thread::sleep_for(1s);

	SPtr<Network::ClientService> service = Memory::MakeShared<Network::ClientService>(
		Network::SockaddrWrapper(L"127.0.0.1", 7777),
		Memory::MakeShared<Client::Network::ServerSession>,
		Memory::MakeShared<Network::IocpEventForwarder>());
	
	assert(service->Launch());
	std::cout << "Launched client service" << std::endl;

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

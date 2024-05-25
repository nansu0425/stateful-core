// app-client/main/Main.cpp

#include <app-client/precompiled/Pch.hpp>
#include <app-client/network/Sessions.hpp>
#include <app-client/network/PacketHandler.hpp>
#include <lib-stateful-core/network/Service.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>
#include <lib-stateful-core/asynchronous/ReservedJobQueue.hpp>
#include <lib-stateful-core/asynchronous/JobQueueManager.hpp>

Memory::String g_roomName;
Memory::String g_message;

int main()
{
	int32 userCnt = 1;

	std::cout << "Room name: ";
	std::getline(std::cin, g_roomName);
	std::cout << "Message: ";
	std::getline(std::cin, g_message);;
	std::cout << "User count: ";
	std::cin >> userCnt;

	SPtr<Network::ClientService> service = Memory::MakeShared<Network::ClientService>(
		Network::SockaddrWrapper(L"127.0.0.1", 7720),
		Memory::MakeShared<Client::Network::ServerSession>,
		Memory::MakeShared<Network::IocpEventForwarder>(),
		userCnt);
	
	bool launched = service->Launch();
	assert(launched);
	std::cout << "Launched client service" << std::endl;

	Client::Network::PacketHandler::Init();
	std::cout << "Initialized packet handler" << std::endl;

	for (int32 i = 0; i < Thread::hardware_concurrency() / 2; i++)
	{
		Multithreading::g_threadManager->Launch([&service]()
			{
				while (true)
				{
					service->GetIocpEventForwarder()->ForwardEvent2Handler();
				}
			});
	}
	std::cout << "Launched iocp event handling threads" << std::endl;

	Multithreading::g_threadManager->Join();

	return 0;
}

// app-server/main/Main.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/network/Sessions.hpp>
#include <app-server/network/PacketHandler.hpp>
#include <lib-stateful-core/network/Service.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>
#include <lib-stateful-core/asynchronous/ReservedJobQueue.hpp>
#include <lib-stateful-core/asynchronous/JobQueueManager.hpp>

enum TickConst : Tick64
{
	MAIN_CYCLE = 64
};

void MainCycleLoop(SPtr<Network::ServerService>& service)
{
	while (true)
	{
		Asynchronous::l_mainCycleEnd = ::GetTickCount64() + TickConst::MAIN_CYCLE;

		// Handle iocp event
		service->GetIocpEventForwarder()->ForwardEvent2Handler(10);

		// Distribute reserved jobs to job queue manager
		Asynchronous::ReservedJobQueue::Distribute2JobQueueManager();

		// Process job queues
		Asynchronous::JobQueueManager::ProcessJobQueues();
	}
}

int main()
{
	SPtr<Network::ServerService> service = Memory::MakeShared<Network::ServerService>(
		Network::SockaddrWrapper(L"127.0.0.1", 7720),
		Memory::MakeShared<Server::Network::ClientSession>,
		Memory::MakeShared<Network::IocpEventForwarder>(),
		100);

	bool launched = service->Launch();
	assert(launched);
	std::cout << "Launched server service" << std::endl;

	Server::Network::PacketHandler::Init();
	std::cout << "Initialized packet handler" << std::endl;

	for (int32 i = 0; i < 4; i++)
	{
		Multithreading::g_threadManager->Launch([&service]()
			{
				MainCycleLoop(service);
			});
	}
	std::cout << "Launched main cycle loop threads" << std::endl;

	Multithreading::g_threadManager->Join();

	return 0;
}

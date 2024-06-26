// app-client/main/Main.cpp

#include <app-client/precompiled/Pch.hpp>
#include <app-client/network/Sessions.hpp>
#include <app-client/network/PacketHandler.hpp>
#include <lib-stateful-core/network/Service.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>
#include <lib-stateful-core/asynchronous/ReservedJobQueue.hpp>
#include <lib-stateful-core/asynchronous/JobQueueManager.hpp>

const uint64 NUM_CLIENTS = 10;

enum TickConst : Tick64
{
	MAIN_CYCLE = 64
};

void MainCycleLoop(SPtr<Network::ClientService>& service)
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
	std::this_thread::sleep_for(1s);

	SPtr<Network::ClientService> service = Memory::MakeShared<Network::ClientService>(
		Network::SockaddrWrapper(L"127.0.0.1", 7720),
		Memory::MakeShared<Client::Network::ServerSession>,
		Memory::MakeShared<Network::IocpEventForwarder>(),
		NUM_CLIENTS);
	
	bool launched = service->Launch();
	assert(launched);
	std::cout << "Launched client service" << std::endl;

	Client::Network::PacketHandler::Init();
	std::cout << "Initialized packet handler" << std::endl;

	for (int32 i = 0; i < 4; i++)
	{
		Multithreading::g_threadManager->Launch([&service]()
			{
				MainCycleLoop(service);
			});
	}
	std::cout << "Launched main cycle loop threads" << std::endl;

	Multithreading::g_threadManager->Launch([&service]()
		{
			while (true)
			{
				Packet::C_ECHO pktSend;
				pktSend.set_msg(std::string("Echo message"));

				auto sendBufChunk = Client::Network::PacketHandler::Serialize2SendBufChunk(pktSend);
				service->Broadcast(sendBufChunk);

				std::this_thread::sleep_for(1s);
			}
		});

	Multithreading::g_threadManager->Join();

	return 0;
}

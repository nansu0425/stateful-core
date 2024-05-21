// app-client/main/Main.cpp

#include <app-client/precompiled/Pch.hpp>
#include <app-client/network/Sessions.hpp>
#include <lib-stateful-core/network/Service.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>

int main()
{
	std::this_thread::sleep_for(1s);

	std::cout << "Client started..." << std::endl;

	SPtr<Network::ClientService> service = Memory::MakeShared<Network::ClientService>(
		Network::SockaddrWrapper(L"127.0.0.1", 7777),
		Memory::MakeShared<Client::Network::ServerSession>,
		Memory::MakeShared<Network::IocpEventForwarder>(), 
		10);
	
	assert(service->Launch());

	for (int32 i = 0; i < 4; i++)
	{
		Multithreading::g_threadManager->Launch([=]()
			{
				while (true)
					service->GetIocpEventForwarder()->ForwardEvent2Handler();
			});
	}

	std::this_thread::sleep_for(1s);

	const char msgSend[] = "Test message";

	Multithreading::g_threadManager->Launch([=]()
		{
			while (true)
			{
				SPtr<Network::SendBufferChunk> sendBufChunk =
					Network::g_sendBufferManager->StartWritingChunk(4096);
				byte* chunk = sendBufChunk->GetChunk();
				
				reinterpret_cast<Network::PacketHeader*>(chunk)->size
					= sizeof(Network::PacketHeader) + sizeof(msgSend);
				reinterpret_cast<Network::PacketHeader*>(chunk)->type
					= 1;

				::memcpy_s(&chunk[4], 4096 - 4, msgSend, sizeof(msgSend));
				sendBufChunk->EndWriting(sizeof(Network::PacketHeader) + sizeof(msgSend));

				service->Broadcast(sendBufChunk);
				std::this_thread::sleep_for(1s);
			}
		});

	Multithreading::g_threadManager->Join();

	return 0;
}

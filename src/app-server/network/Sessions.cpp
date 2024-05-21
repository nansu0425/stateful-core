// app-server/network/Sessions.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/network/Sessions.hpp>
#include <lib-stateful-core/network/SendBufferManager.hpp>
#include <lib-stateful-core/network/Service.hpp>

namespace Server
{
	namespace Network
	{
		void ClientSession::ProcessConnect()
		{
			std::cout << "Connected to client!" << std::endl;
		}

		void ClientSession::ProcessDisconnect()
		{
			std::cout << "Disconnected to client..." << std::endl;
		}

		void ClientSession::ProcessPacketRecv(byte* buf, int32 packetSize)
		{
			std::cout << "Received packet(" << packetSize << "B)" << std::endl;
		}

		void ClientSession::ProcessSend(int32 numBytesSent)
		{
			std::cout << "Sent data(" << numBytesSent << "B)" << std::endl;
		}
	}
}

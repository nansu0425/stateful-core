// app-client/network/Sessions.cpp

#include <app-client/precompiled/Pch.hpp>
#include <app-client/network/Sessions.hpp>

namespace Client
{
	namespace Network
	{
		void ServerSession::ProcessConnect()
		{
			std::cout << "Connected to server!" << std::endl;
		}

		void ServerSession::ProcessDisconnect()
		{
			std::cout << "Disconnected to server..." << std::endl;
		}

		void ServerSession::ProcessPacketRecv(byte* buf, int32 packetSize)
		{
			std::cout << "Received packet(" << packetSize << "B)" << std::endl;
		}

		void ServerSession::ProcessSend(int32 numBytesSent)
		{
			std::cout << "Sent data(" << numBytesSent << "B)" << std::endl;
		}
	}
}

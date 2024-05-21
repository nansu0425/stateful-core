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
			namespace CoreNet = StatefulCore::Network;

			CoreNet::PacketHeader header = *(reinterpret_cast<CoreNet::PacketHeader*>(buf));
			
			char msgRecv[4096] = {};
			::memcpy_s(msgRecv, 4096, 
				&buf[sizeof(CoreNet::PacketHeader)], header.size - sizeof(CoreNet::PacketHeader));

			std::cout << "Received message[\"" << msgRecv << "\"]" << std::endl;
		}

		void ServerSession::ProcessSend(int32 numBytesSent)
		{
			std::cout << "Sent " << numBytesSent << "B" << std::endl;
		}
	}
}

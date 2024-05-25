// app-client/network/Sessions.cpp

#include <app-client/precompiled/Pch.hpp>
#include <app-client/network/Sessions.hpp>
#include <app-client/network/PacketHandler.hpp>

extern Memory::String g_roomName;

namespace Client
{
	namespace Network
	{
		void ServerSession::ProcessConnect()
		{
			std::cout << "Connected to server!" << std::endl;

			Packet::C_CREATE_ROOM pktSend;
			pktSend.set_room_name(g_roomName.c_str());

			auto sendBufChunk = PacketHandler::Serialize2SendBufChunk(pktSend);
			Send(sendBufChunk);
		}

		void ServerSession::ProcessDisconnect()
		{
			std::cout << "Disconnected to server..." << std::endl;
		}

		void ServerSession::ProcessPacketRecv(byte* buf, int32 packetSize)
		{
			// std::cout << "Received packet(" << packetSize << "B)" << std::endl;

			SPtr<StatefulCore::Network::PacketSession> session = GetShared();

			assert(packetSize >= sizeof(StatefulCore::Network::PacketHeader));
			PacketHandler::HandlePacket(session, buf, packetSize);
		}

		void ServerSession::ProcessSend(int32 numBytesSent)
		{
			// std::cout << "Sent data(" << numBytesSent << "B)" << std::endl;
		}
	}
}

// app-server/network/Sessions.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/network/Sessions.hpp>
#include <app-server/network/PacketHandler.hpp>
#include <app-server/contents/Room.hpp>
#include <app-server/contents/User.hpp>

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

			SPtr<Server::Contents::Room> room = m_user->GetOwner();
			room->DoAsync(&Server::Contents::Room::Exit, m_user->GetId());
		}

		void ClientSession::ProcessPacketRecv(byte* buf, int32 packetSize)
		{
			std::cout << "Received packet(" << packetSize << "B)" << std::endl;

			SPtr<StatefulCore::Network::PacketSession> session = GetShared();

			assert(packetSize >= sizeof(StatefulCore::Network::PacketHeader));
			PacketHandler::HandlePacket(session, buf, packetSize);
		}

		void ClientSession::ProcessSend(int32 numBytesSent)
		{
			std::cout << "Sent data(" << numBytesSent << "B)" << std::endl;
		}
	}
}

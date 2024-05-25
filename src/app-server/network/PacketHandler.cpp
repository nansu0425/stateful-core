// app-server/network/PacketHandler.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/network/PacketHandler.hpp>
#include <app-server/network/Sessions.hpp>
#include <app-server/contents/RoomManager.hpp>
#include <app-server/contents/User.hpp>
#include <app-server/contents/Room.hpp>

namespace Server
{
	namespace Network
	{
		PacketHandlerFunc g_packetHandlerTable[UINT16_MAX];

		bool Process_INVALID(SPtr<CorePktSession>& session, byte* buffer, uint32 size)
		{
			std::cout << "INVALID " << size << "B" << std::endl;

			return true;
		}

		bool Process_C_CREATE_ROOM(SPtr<CorePktSession>& session, Packet::C_CREATE_ROOM& packet)
		{
			Packet::S_CREATE_ROOM pktSend;

			if (packet.async_completion())
			{
				if (packet.cause_fail() & Packet::C_CREATE_ROOM_FAIL_TYPE_EXIST)
					pktSend.set_cause_fail(Packet::S_CREATE_ROOM_FAIL_TYPE_EXIST);

				if (Server::Contents::g_roomManager->ExistName(packet.room_name().c_str()) == false)
					return false;

				pktSend.set_room_id(
					Server::Contents::g_roomManager->GetId(packet.room_name().c_str()));

				std::cout << "Created " << packet.room_name() << std::endl;
			}
			else
			{
				Server::Contents::g_roomManager->DoAsync(
					&Contents::RoomManager::Create, session, packet);

				pktSend.set_room_id(-1);
				pktSend.set_cause_fail(Packet::S_CREATE_ROOM_FAIL_TYPE_ASYNC);
			}

			auto sendBufChunk = PacketHandler::Serialize2SendBufChunk(pktSend);
			session->Send(sendBufChunk);

			return true;
		}

		bool Process_C_ENTER_ROOM(SPtr<CorePktSession>& session, Packet::C_ENTER_ROOM& packet)
		{
			Packet::S_ENTER_ROOM pktSend;

			if (packet.async_completion())
			{
				pktSend.set_user_id(
					std::static_pointer_cast<Network::ClientSession>(session)->GetUser()->GetId());

				auto room = Server::Contents::g_roomManager->GetRoom(packet.room_id());
				std::cout << "User " << pktSend.user_id() << " entered " << room->GetName() << std::endl;
			}
			else
			{
				if (Server::Contents::g_roomManager->ExistId(packet.room_id()) == false)
				{
					pktSend.set_cause_fail(Packet::S_ENTER_ROOM_FAIL_TYPE_NOT_EXIST);
				}
				else
				{
					auto room = Server::Contents::g_roomManager->GetRoom(packet.room_id());
					room->DoAsync(&Contents::Room::Enter, session, packet);

					pktSend.set_cause_fail(Packet::S_ENTER_ROOM_FAIL_TYPE_ASYNC);
				}

				pktSend.set_user_id(-1);
			}

			auto sendBufChunk = PacketHandler::Serialize2SendBufChunk(pktSend);
			session->Send(sendBufChunk);

			return true;
		}

		bool Process_C_CHAT(SPtr<CorePktSession>& session, Packet::C_CHAT& packet)
		{
			auto clientSession = std::static_pointer_cast<Network::ClientSession>(session);

			std::cout << clientSession->GetUser()->GetOwner()->GetName() << ": User " 
				<< clientSession->GetUser()->GetId() << ": " << packet.msg() << std::endl;

			Packet::S_CHAT pktSend;
			pktSend.set_user_id(clientSession->GetUser()->GetId());
			pktSend.set_msg(packet.msg());

			auto sendBufChunk = PacketHandler::Serialize2SendBufChunk(pktSend);			
			clientSession->GetUser()->DoAsync(1000, &Contents::User::Send, sendBufChunk);

			return true;
		}
	}
}

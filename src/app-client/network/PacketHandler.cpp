// app-client/network/PacketHandler.cpp

#include <app-client/precompiled/Pch.hpp>
#include <app-client/network/PacketHandler.hpp>

extern Memory::String g_roomName;
extern Memory::String g_message;

namespace Client
{
	namespace Network
	{
		PacketHandlerFunc g_packetHandlerTable[UINT16_MAX];

		bool Process_INVALID(SPtr<CorePktSession>& session, byte* buffer, uint32 size)
		{
			std::cout << "INVALID " << size << "B" << std::endl;

			return true;
		}

		bool Process_S_CREATE_ROOM(SPtr<CorePktSession>& session, Packet::S_CREATE_ROOM& packet)
		{
			if (packet.cause_fail() != Packet::S_CREATE_ROOM_FAIL_TYPE_ASYNC)
			{
				std::cout << "Created " << g_roomName << std::endl;

				Packet::C_ENTER_ROOM pktSend;
				pktSend.set_room_id(packet.room_id());

				auto sendBufChunk = PacketHandler::Serialize2SendBufChunk(pktSend);
				session->Send(sendBufChunk);
			}

			return true;
		}

		bool Process_S_ENTER_ROOM(SPtr<CorePktSession>& session, Packet::S_ENTER_ROOM& packet)
		{
			if (packet.cause_fail() == Packet::S_ENTER_ROOM_FAIL_TYPE_UNSPEC)
			{
				std::cout << "User " << packet.user_id() << " entered " << g_roomName << std::endl;

				Packet::C_CHAT pktSend;
				pktSend.set_msg(g_message.c_str());

				auto sendBufChunk = PacketHandler::Serialize2SendBufChunk(pktSend);
				session->Send(sendBufChunk);
			}

			return true;
		}

		bool Process_S_CHAT(SPtr<CorePktSession>& session, Packet::S_CHAT& packet)
		{
			std::cout << g_roomName << ": User " << packet.user_id() << ": " << packet.msg() << std::endl;

			Packet::C_CHAT pktSend;
			pktSend.set_msg(g_message.c_str());

			auto sendBufChunk = PacketHandler::Serialize2SendBufChunk(pktSend);
			session->Send(sendBufChunk);

			return true;
		}
	}
}
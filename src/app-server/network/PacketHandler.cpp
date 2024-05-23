// app-server/network/PacketHandler.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/network/PacketHandler.hpp>
#include <lib-stateful-core/network/Service.hpp>

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

		bool Process_C_ECHO(SPtr<CorePktSession>& session, Packet::C_ECHO& packet)
		{
			std::cout << "C_ECHO [" << packet.msg() << "]" << std::endl;

			Packet::S_ECHO pktSend;
			pktSend.set_msg(packet.msg());

			auto sendBufChunk = PacketHandler::Serialize2SendBufChunk(pktSend);
			session->GetOwner()->Broadcast(sendBufChunk);

			return true;
		}
	}
}
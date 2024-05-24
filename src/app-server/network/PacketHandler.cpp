// app-server/network/PacketHandler.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/network/PacketHandler.hpp>

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
			return true;
		}

		bool Process_C_ENTER_ROOM(SPtr<CorePktSession>& session, Packet::C_ENTER_ROOM& packet)
		{
			return true;
		}

		bool Process_C_CHAT(SPtr<CorePktSession>& session, Packet::C_CHAT& packet)
		{
			return true;
		}
	}
}
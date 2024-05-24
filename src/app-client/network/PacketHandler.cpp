// app-client/network/PacketHandler.cpp

#include <app-client/precompiled/Pch.hpp>
#include <app-client/network/PacketHandler.hpp>

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
			return true;
		}

		bool Process_S_ENTER_ROOM(SPtr<CorePktSession>& session, Packet::S_ENTER_ROOM& packet)
		{
			return true;
		}

		bool Process_S_CHAT(SPtr<CorePktSession>& session, Packet::S_CHAT& packet)
		{
			return true;
		}
	}
}
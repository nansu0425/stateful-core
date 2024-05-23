// app-client/network/PacketHandler.cpp

#include <app-client/precompiled/Pch.hpp>
#include <app-client/network/PacketHandler.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>

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


		bool Process_S_ECHO(SPtr<CorePktSession>& session, Packet::S_ECHO& packet)
		{
			std::cout << "S_ECHO [" << packet.msg() << "]" << std::endl;

			return true;
		}
	}
}
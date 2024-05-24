// app-client/network/PacketHandler.hpp

#pragma once

#include <app-client/network/Packet.pb.h>

namespace Client
{
	namespace Network
	{
		using CorePktSession = StatefulCore::Network::PacketSession;
		using PacketHandlerFunc = std::function<bool(SPtr<CorePktSession>&, byte*, uint32)>;
		
		extern PacketHandlerFunc g_packetHandlerTable[UINT16_MAX];

		namespace PacketHandlerIdx
		{
			enum : uint16
			{
				C_ECHO = 1000,
				S_ECHO = 1001,
			};
		}

		// Packet processing functions
		bool Process_INVALID(SPtr<CorePktSession>& session, byte* buffer, uint32 size);
		bool Process_S_ECHO(SPtr<CorePktSession>& session, Packet::S_ECHO& packet);

		/*---------------------*
		 *    PacketHandler    *
		 *---------------------*/

		class PacketHandler
		{
		private:
			using Header = StatefulCore::Network::PacketHeader;
			using SendBufChunk = StatefulCore::Network::SendBufferChunk;

			enum : uint32
			{
				MAX_PKT_SIZE = 4096
			};

		public:
			static void Init()
			{
				for (int32 i = 0; i < UINT16_MAX; i++)
					g_packetHandlerTable[i] = Process_INVALID;

				g_packetHandlerTable[PacketHandlerIdx::S_ECHO] = 
					[](SPtr<CorePktSession>& session, byte* buffer, uint32 size)
					{
						return HandlePacket<Packet::S_ECHO>(
							Process_S_ECHO, session, buffer, size);
					};
			}

			static bool HandlePacket(SPtr<CorePktSession>& session, byte* buffer, uint32 size)
			{
				Header* header = reinterpret_cast<Header*>(buffer);

				return g_packetHandlerTable[header->idx](session, buffer, size);
			}

			static SPtr<SendBufChunk> Serialize2SendBufChunk(Packet::C_ECHO& packet)
			{
				return Serialize2SendBufChunk(packet, PacketHandlerIdx::C_ECHO);
			}

		private:
			template<typename PacketType, typename FuncProcess>
			static bool HandlePacket(
				FuncProcess processor, SPtr<CorePktSession>& session, byte* buffer, uint32 size)
			{
				PacketType packet;

				assert(size >= sizeof(Header));
				if (packet.ParseFromArray(buffer + sizeof(Header), size - sizeof(Header))
					== false)
					return false;

				return processor(session, packet);
			}

			template<typename PacketType>
			static SPtr<SendBufChunk> Serialize2SendBufChunk(PacketType& packet, uint16 pktHandlerIdx)
			{
				const uint16 dataSize = static_cast<uint16>(packet.ByteSizeLong());
				const uint16 packetSize = dataSize + sizeof(Header);

				assert(packetSize <= MAX_PKT_SIZE);
				SPtr<SendBufChunk> sendBufChunk =
					StatefulCore::Network::g_sendBufferManager->StartWritingChunk(MAX_PKT_SIZE);

				Header* header = reinterpret_cast<Header*>(sendBufChunk->GetChunk());
				header->size = packetSize;
				header->idx = pktHandlerIdx;

				bool serialized = packet.SerializeToArray(&header[1], dataSize);
				assert(serialized);
				sendBufChunk->EndWriting(packetSize);

				return sendBufChunk;
			}
		};
	}
}
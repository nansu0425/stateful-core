{% if parser.recv_prefix == 'C_' -%}
// app-server/network/PacketHandler.hpp
{%- elif parser.recv_prefix == 'S_' -%}
// app-client/network/PacketHandler.hpp
{%- endif %}

#pragma once

{% if parser.recv_prefix == 'C_' -%}
#include <app-server/network/Packet.pb.h>
{%- elif parser.recv_prefix == 'S_' -%}
#include <app-client/network/Packet.pb.h>
{%- endif %}

{% if parser.recv_prefix == 'C_' -%}
namespace Server
{%- elif parser.recv_prefix == 'S_' -%}
namespace Client
{%- endif %}
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
{%- for pkt in parser.total_pkts %}
				{{pkt.name}} = {{pkt.id}},
{%- endfor %}
			};
		}

		// Packet processing functions
		bool Process_INVALID(SPtr<CorePktSession>& session, byte* buffer, uint32 size);
{%- for pkt in parser.recv_pkts %}
		bool Process_{{pkt.name}}(SPtr<CorePktSession>& session, Packet::{{pkt.name}}& packet);
{%- endfor %}

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

{%- for pkt in parser.recv_pkts %}

				g_packetHandlerTable[PacketHandlerIdx::{{pkt.name}}] = 
					[](SPtr<CorePktSession>& session, byte* buffer, uint32 size)
					{
						return HandlePacket<Packet::{{pkt.name}}>(
							Process_{{pkt.name}}, session, buffer, size);
					};

{%- endfor %}
			}

			static bool HandlePacket(SPtr<CorePktSession>& session, byte* buffer, uint32 size)
			{
				Header* header = reinterpret_cast<Header*>(buffer);

				return g_packetHandlerTable[header->idx](session, buffer, size);
			}

{%- for pkt in parser.send_pkts %}

			static SPtr<SendBufChunk> Serialize2SendBufChunk(Packet::{{pkt.name}}& packet)
			{
				return Serialize2SendBufChunk(packet, PacketHandlerIdx::{{pkt.name}});
			}
	
{%- endfor %}

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

				assert(packet.SerializeToArray(&header[1], dataSize));
				sendBufChunk->EndWriting(packetSize);

				return sendBufChunk;
			}
		};
	}
}

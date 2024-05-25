// app-server/contents/RoomManager.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/contents/RoomManager.hpp>
#include <app-server/contents/Room.hpp>
#include <app-server/network/PacketHandler.hpp>

namespace Server
{
	namespace Contents
	{
		void RoomManager::Create(SPtr<PktSession> session, Packet::C_CREATE_ROOM packet)
		{
			Memory::String roomName(packet.room_name().c_str());

			if (m_roomNames.find(roomName) != m_roomNames.end())
			{
				packet.set_cause_fail(Packet::C_CREATE_ROOM_FAIL_TYPE_EXIST);
			}
			else
			{
				Id roomId = m_nextRoomId++;
				m_roomNames.insert(std::pair<Memory::String, Id>(roomName, roomId));

				SPtr<Room> room = Memory::ObjectPool<Room>::MakeShared(roomId, roomName);
				m_rooms.insert(std::pair<Id, SPtr<Room>>(roomId, room));

				m_rooms[roomId]->SetOwner(GetShared());
			}

			packet.set_async_completion(true);
			Server::Network::Process_C_CREATE_ROOM(session, packet);
		}

		void RoomManager::Release(Id roomId)
		{
			m_roomNames.erase(m_rooms[roomId]->GetName());
			m_rooms.erase(roomId);
		}

		SPtr<RoomManager> g_roomManager = Memory::MakeShared<RoomManager>();
	}
}

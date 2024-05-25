// app-server/contents/RoomManager.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/contents/RoomManager.hpp>
#include <app-server/contents/Room.hpp>

namespace Server
{
	namespace Contents
	{
		bool RoomManager::Create(OUT Id& roomId, Memory::String name)
		{
			if (m_roomNames.find(name) != m_roomNames.end())
				return false;

			roomId = m_nextRoomId++;
			SPtr<Room> room = Memory::ObjectPool<Room>::MakeShared(roomId, name);
			m_rooms.insert(std::pair<Id, SPtr<Room>>(roomId, room));

			return true;
		}

		void RoomManager::Release(Id roomId)
		{
			m_roomNames.erase(m_rooms[roomId]->GetName());
			m_rooms.erase(roomId);
		}
	}
}

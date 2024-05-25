// app-server/contents/RoomManager.hpp

#pragma once

namespace Packet
{
	class C_CREATE_ROOM;
}

namespace Server
{
	namespace Contents
	{
		class Room;

		/*-------------------*
		 *    RoomManager    *
		 *-------------------*/

		class RoomManager : public Asynchronous::JobQueue
		{
		private:
			using Id = int32;
			using PktSession = StatefulCore::Network::PacketSession;

		public:
			void    Create(SPtr<PktSession> session, Packet::C_CREATE_ROOM packet);
			void    Release(Id roomId);

			SPtr<Room>    GetRoom(Id roomId) { return m_rooms[roomId]; }
			Id            GetId(Memory::String roomName) { return m_roomNames[roomName]; }
			SPtr<RoomManager>    GetShared() { return std::static_pointer_cast<RoomManager>(shared_from_this()); }

			bool    ExistId(Id roomId) { return m_rooms.find(roomId) != m_rooms.end(); }
			bool    ExistName(Memory::String name) { return m_roomNames.find(name) != m_roomNames.end(); }

		private:
			Id      m_nextRoomId = 0;
			Memory::HashMap<Id, SPtr<Room>>        m_rooms;
			Memory::HashMap<Memory::String, Id>    m_roomNames;
		};

		extern SPtr<RoomManager> g_roomManager;
	}
}

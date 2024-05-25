// app-server/contents/RoomManager.hpp

#pragma once

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
			bool    Create(OUT Id& roomId, Memory::String name);
			void    Release(Id roomId);

		private:
			Id      m_nextRoomId = 0;
			Memory::HashMap<Id, SPtr<Room>>    m_rooms;
			Memory::HashSet<Memory::String>    m_roomNames;
		};
	}
}

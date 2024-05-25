// app-server/contents/Room.hpp

#pragma once

namespace Server
{
	namespace Contents
	{
		class User;
		class RoomManager;

		/*------------*
		 *    Room    *
		 *------------*/

		class Room : public Asynchronous::JobQueue
		{
		private:
			using Id = int32;
			using PktSession = StatefulCore::Network::PacketSession;

		public:
			Room(Id roomId, Memory::String name) : m_roomId(roomId), m_name(name) { }
			~Room() { std::cout << "~Room()" << std::endl; }

		public:
			bool    Enter(SPtr<PktSession> pktSession);
			void    Exit(Id userId);
			void    Broadcast(SPtr<StatefulCore::Network::SendBufferChunk> sendBufChunk);

			Id                GetRoomId() { return m_roomId; }
			Memory::String    GetName() { return m_name; }
			SPtr<Room>        GetShared() { return std::static_pointer_cast<Room>(shared_from_this()); }

		private:
			Id    m_roomId;
			Id    m_nextUserId = 0;

			Memory::String    m_name;
			Memory::HashMap<Id, SPtr<User>>    m_users;
			
			WPtr<RoomManager> m_owner;
		};
	}
}
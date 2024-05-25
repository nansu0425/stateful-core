// app-server/contents/User.hpp

namespace Server
{
	namespace Network
	{
		class ClientSession;
	}

	namespace Contents
	{
		class Room;

		/*------------*
		 *    User    *
		 *------------*/

		class User
		{
		private:
			using Id = int32;

		public:
			User(Id id, SPtr<Room> owner) : m_id(id), m_owner(owner) { }
			~User() { std::cout << "~User()" << std::endl; }

			Id            GetId() { return m_id; }
			SPtr<Room>    GetOwner() { return m_owner.lock(); }
			SPtr<Network::ClientSession>    GetSession() { return m_session.lock(); }

			void          SetSession(SPtr<Network::ClientSession> session) { m_session = session; }

		private:
			Id            m_id;
			WPtr<Room>    m_owner;
			WPtr<Network::ClientSession>    m_session;
		};
	}
}

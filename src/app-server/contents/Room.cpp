// app-server/contents/Room.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/contents/Room.hpp>
#include <app-server/network/Sessions.hpp>
#include <app-server/contents/User.hpp>
#include <app-server/contents/RoomManager.hpp>

namespace Server
{
	namespace Contents
	{
		bool Room::Enter(SPtr<PktSession> pktSession)
		{
			auto clientSession 
				= std::static_pointer_cast<Server::Network::ClientSession>(pktSession);
			
			SPtr<User> user = Memory::ObjectPool<User>::MakeShared(m_nextUserId++, GetShared());
			clientSession->SetUser(user);

			auto res = m_users.insert(std::pair<Id, SPtr<User>>(user->GetId(), user));

			return res.second;
		}

		void Room::Exit(Id userId)
		{
			uint64 numErased = m_users.erase(userId);

			if (m_users.size() == 0)
			{
				SPtr<RoomManager> owner = m_owner.lock();
				owner->DoAsync(&RoomManager::Release, m_roomId);
			}
		}

		void Room::Broadcast(SPtr<StatefulCore::Network::SendBufferChunk> sendBufChunk)
		{
			for (auto& user : m_users)
			{
				SPtr<Server::Network::ClientSession> session = user.second->GetSession();
				
				if (session)
					session->Send(sendBufChunk);
			}
		}
	}
}

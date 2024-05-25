// app-server/contents/User.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/contents/User.hpp>
#include <app-server/network/Sessions.hpp>

namespace Server
{
	namespace Contents
	{
		void User::Send(SPtr<StatefulCore::Network::SendBufferChunk> sendBufChunk)
		{
			auto session = GetSession();
			
			if (session)
				session->Send(sendBufChunk);
		}
	}
}

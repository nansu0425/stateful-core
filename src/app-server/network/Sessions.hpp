// app-server/network/Sessions.hpp

#pragma once

namespace Server
{
	namespace Contents
	{
		class User;
	}

	namespace Network
	{
		/*---------------------*
		 *    ClientSession    *
		 *---------------------*/

		class ClientSession : public StatefulCore::Network::PacketSession
		{
		public:
			~ClientSession()
			{
				std::cout << "~ClientSession()" << std::endl;
			}

		public:
			void    SetUser(SPtr<Server::Contents::User> user) { m_user = user; }

		protected:
			virtual void    ProcessConnect() override;
			virtual void    ProcessDisconnect() override;
			virtual void    ProcessPacketRecv(byte* buf, int32 packetSize) override;
			virtual void    ProcessSend(int32 numBytesSent) override;

		private:
			SPtr<Server::Contents::User>    m_user;
		};
	}
}

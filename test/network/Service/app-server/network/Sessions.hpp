// app-server/network/Sessions.hpp

#pragma once

namespace Server
{
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

		protected:
			virtual void    ProcessConnect() override;
			virtual void    ProcessDisconnect() override;
			virtual void    ProcessPacketRecv(byte* buf, int32 packetSize) override;
			virtual void    ProcessSend(int32 numBytesSent) override;
		};

		/*----------------------------*
		 *    ClientSessionManager    *
		 *----------------------------*/
		
		class ClientSessionManager
		{
		private:
			using SendBufChunk = StatefulCore::Network::SendBufferChunk;

		public:
			void    Insert(SPtr<ClientSession> session);
			void    Erase(SPtr<ClientSession> session);
			void    Broadcast(SPtr<SendBufChunk> sendBufChunk);

		private:
			USE_RW_SPIN_LOCK;
			HashSet<SPtr<ClientSession>>    m_sessions;
		};
		
		extern ClientSessionManager    g_clientSessionManager;
	}
}

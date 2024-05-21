// app-client/network/Sessions.hpp

#pragma once

namespace Client
{
	namespace Network
	{
		/*---------------------*
		 *    ServerSession    *
		 *---------------------*/

		class ServerSession : public StatefulCore::Network::PacketSession
		{
		public:
			~ServerSession()
			{
				std::cout << "~ServerSession()" << std::endl;
			}

		protected:
			virtual void    ProcessConnect() override;
			virtual void    ProcessDisconnect() override;
			virtual void    ProcessPacketRecv(byte* buf, int32 packetSize) override;
			virtual void    ProcessSend(int32 numBytesSent) override;
		};
	}
}

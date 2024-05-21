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
	}
}

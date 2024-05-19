// lib-stateful-core/network/SocketAddress.hpp

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		/*---------------------*
		 *    SocketAddress    *
		 *---------------------*/

		class SocketAddress
		{
		public:
			SocketAddress() = default;
			SocketAddress(SOCKADDR_IN sockAddr) : m_sockAddr(sockAddr) { }
			SocketAddress(Memory::WString ip, uint16 port);

		public:
			SOCKADDR_IN&       GetSockAddr() { return m_sockAddr; }
			Memory::WString    GetIpAddr();
			uint16             GetPort() { return ::ntohs(m_sockAddr.sin_port); }

		private:
			SOCKADDR_IN m_sockAddr = {};
		};

		IN_ADDR Ip2Addr(const WCHAR* ip);
	}
}

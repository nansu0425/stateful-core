// lib-stateful-core/network/SocketAddress.hpp

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		/*-----------------------*
		 *    SockaddrWrapper    *
		 *-----------------------*/

		class SockaddrWrapper
		{
		public:
			SockaddrWrapper() = default;
			SockaddrWrapper(SOCKADDR_IN sockaddr) : m_sockaddr(sockaddr) { }
			SockaddrWrapper(Memory::WString ip, uint16 port);

		public:
			SOCKADDR_IN&       GetSockaddr() { return m_sockaddr; }
			Memory::WString    GetIp();
			uint16             GetPort() { return ::ntohs(m_sockaddr.sin_port); }

		private:
			SOCKADDR_IN m_sockaddr = {};
		};

		IN_ADDR Ip2Addr(const WCHAR* ip);
	}
}

// lib-stateful-core/network/SocketAddress.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/network/SockaddrWrapper.hpp>

namespace StatefulCore
{
	namespace Network
	{
		SockaddrWrapper::SockaddrWrapper(Memory::WString ip, uint16 port)
		{
			::memset(&m_sockaddr, 0, sizeof(m_sockaddr));
			m_sockaddr.sin_family = AF_INET;
			m_sockaddr.sin_addr = Ip2Addr(ip.c_str());
			m_sockaddr.sin_port = ::htons(port);
		}

		Memory::WString SockaddrWrapper::GetIp()
		{
			WCHAR buffer[100];
			::InetNtopW(AF_INET, &m_sockaddr.sin_addr, buffer, LENTH_32(buffer));
			return Memory::WString(buffer);
		}

		IN_ADDR Ip2Addr(const WCHAR* ip)
		{
			IN_ADDR addr;
			::InetPtonW(AF_INET, ip, &addr);
			return addr;
		}
	}
}
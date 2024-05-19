// lib-stateful-core/network/SocketAddress.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/network/SocketAddress.hpp>

namespace StatefulCore
{
	namespace Network
	{
		SocketAddress::SocketAddress(Memory::WString ip, uint16 port)
		{
			::memset(&m_sockAddr, 0, sizeof(m_sockAddr));
			m_sockAddr.sin_family = AF_INET;
			m_sockAddr.sin_addr = Ip2Addr(ip.c_str());
			m_sockAddr.sin_port = ::htons(port);
		}

		Memory::WString SocketAddress::GetIpAddr()
		{
			WCHAR buffer[100];
			::InetNtopW(AF_INET, &m_sockAddr.sin_addr, buffer, LENTH_32(buffer));
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
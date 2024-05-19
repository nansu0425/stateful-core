// lib-stateful-core/network/Socket.hpp

#include <lib-stateful-core/network/SockaddrWrapper.hpp>

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		/*-----------------------*
		 *    SocketFunctions    *
		 *-----------------------*/

		extern LPFN_CONNECTEX       ConnectEx;
		extern LPFN_DISCONNECTEX    DisconnectEx;
		extern LPFN_ACCEPTEX        AcceptEx;

		void Init();
		void Clear();

		bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);
		SOCKET CreateSocket();

		bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
		bool SetReuseAddress(SOCKET socket, bool flag);
		bool SetRecvBufferSize(SOCKET socket, int32 size);
		bool SetSendBufferSize(SOCKET socket, int32 size);
		bool SetTcpNoDelay(SOCKET socket, bool flag);
		bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

		bool Bind(SOCKET socket, SockaddrWrapper sockaddrWrapper);
		bool BindAnyAddress(SOCKET socket, uint16 port);
		
		bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
		void Close(SOCKET& socket);

		template<typename T>
		static inline bool SetSockOpt(SOCKET socket, int32 level, int32 optName, T optVal)
		{
			return ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T))
				!= SOCKET_ERROR;
		}
	}
}

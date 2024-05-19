// lib-stateful-core/network/Socket.hpp

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		/*--------------*
		 *    Socket    *
		 *--------------*/

		class Socket
		{
		public:
			static LPFN_CONNECTEX		s_connectEx;
			static LPFN_DISCONNECTEX	s_disConnectEx;
			static LPFN_ACCEPTEX		s_acceptEx;

		public:
			static void Init();
			static void Clear();

			static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);
			static SOCKET CreateSocket();

			static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
			static bool SetReuseAddress(SOCKET socket, bool flag);
			static bool SetRecvBufferSize(SOCKET socket, int32 size);
			static bool SetSendBufferSize(SOCKET socket, int32 size);
			static bool SetTcpNoDelay(SOCKET socket, bool flag);
			static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

			static bool Bind(SOCKET socket, NetAddress netAddr);
			static bool BindAnyAddress(SOCKET socket, uint16 port);
			static bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
			static void Close(SOCKET& socket);
		};

		template<typename T>
		static inline bool SetSockOpt(SOCKET socket, int32 level, int32 optName, T optVal)
		{
			return ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T))
				!= SOCKET_ERROR;
		}
	}
}

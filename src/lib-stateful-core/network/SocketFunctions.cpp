// lib-stateful-core/network/Socket.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/network/SocketFunctions.hpp>

namespace StatefulCore
{
	namespace Network
	{
		LPFN_CONNECTEX       ConnectEx = nullptr;
		LPFN_DISCONNECTEX    DisconnectEx = nullptr;
		LPFN_ACCEPTEX        AcceptEx = nullptr;

		void Init()
		{
			WSADATA wsaData;
			assert(::WSAStartup(MAKEWORD(2, 2), OUT & wsaData) == 0);

			// Get the function addresses at runtime
			SOCKET dummySocket = CreateSocket();
			assert(BindWindowsFunction(
				dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
			assert(BindWindowsFunction(
				dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
			assert(BindWindowsFunction(
				dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
			Close(dummySocket);
		}

		void Clear()
		{
			::WSACleanup();
		}

		bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
		{
			DWORD bytes = 0;
			return SOCKET_ERROR != ::WSAIoctl(
				socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(fn),
				OUT & bytes, NULL, NULL);
		}

		SOCKET CreateSocket()
		{
			return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
		}

		bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
		{
			LINGER option;
			option.l_onoff = onoff;
			option.l_linger = linger;

			return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, option);
		}

		bool SetReuseAddress(SOCKET socket, bool flag)
		{
			return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
		}

		bool SetRecvBufferSize(SOCKET socket, int32 size)
		{
			return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
		}

		bool SetSendBufferSize(SOCKET socket, int32 size)
		{
			return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
		}

		bool SetTcpNoDelay(SOCKET socket, bool flag)
		{
			return SetSockOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
		}

		bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
		{
			return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
		}

		bool Bind(SOCKET socket, SockaddrWrapper sockaddrWrapper)
		{
			return ::bind(
				socket, reinterpret_cast<const SOCKADDR*>(&sockaddrWrapper.GetSockaddr()), sizeof(SOCKADDR_IN)
			) != SOCKET_ERROR;
		}

		bool BindAnyAddress(SOCKET socket, uint16 port)
		{
			SOCKADDR_IN addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
			addr.sin_port = ::htons(port);

			return ::bind(socket, reinterpret_cast<const SOCKADDR*>(&addr), sizeof(addr)) != SOCKET_ERROR;
		}

		bool Listen(SOCKET socket, int32 backlog)
		{
			return ::listen(socket, backlog) != SOCKET_ERROR;
		}

		void Close(SOCKET& socket)
		{
			if (socket != INVALID_SOCKET)
				::closesocket(socket);

			socket = INVALID_SOCKET;
		}
	}
}
// lib-stateful-core/network/Listener.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/network/Listener.hpp>
#include <lib-stateful-core/network/SocketFunctions.hpp>
#include <lib-stateful-core/network/Service.hpp>

namespace StatefulCore
{
	namespace Network
	{
		Listener::~Listener()
		{
			Close(m_socket);

			for (AcceptEvent* acceptEvent : m_acceptEvents)
			{
				Memory::Delete(acceptEvent);
			}
		}

		bool Listener::StartListen(SPtr<ServerService> serverService)
		{
			m_owner = serverService;
			if (m_owner.lock() == nullptr)
				return false;

			m_socket = CreateSocket();
			if (m_socket == INVALID_SOCKET)
				return false;

			if (serverService->GetIocpEventForwarder()->Associate(shared_from_this()) == false)
				return false;

			if (SetReuseAddress(m_socket, true) == false)
				return false;

			if (SetLinger(m_socket, 0, 0) == false)
				return false;

			if (Bind(m_socket, serverService->GetAddress()) == false)
				return false;

			if (Listen(m_socket) == false)
				return false;

			std::cout << "Started listening..." << std::endl;

			const int32 numMaxSessions = serverService->GetNumMaxSessions();
			for (int32 i = 0; i < numMaxSessions; i++)
			{
				AcceptEvent* acceptEvent = Memory::New<AcceptEvent>();
				acceptEvent->SetHandler(shared_from_this());
				m_acceptEvents.push_back(acceptEvent);
				RegisterAccept(acceptEvent);
			}

			return true;
		}

		void Listener::CloseSocket()
		{
			Close(m_socket);
		}

		void Listener::Handle(IocpEvent* target, int32 numBytesTransferred)
		{
			assert(target->GetType() == IocpEventType::ACCEPT);

			AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(target);
			ProcessAccept(acceptEvent);
		}

		HANDLE Listener::GetHandle()
		{
			return reinterpret_cast<HANDLE>(m_socket);
		}

		void Listener::RegisterAccept(AcceptEvent* acceptEvent)
		{
			SPtr<ServerService> serverService = m_owner.lock();
			assert(serverService != nullptr);

			SPtr<Session> session = serverService->CreateSession();
			acceptEvent->Init();
			acceptEvent->SetHandler(session);

			DWORD numBytesRecv = 0;

			if (AcceptEx(
				m_socket, session->GetSocket(), session->m_recvBuf.GetPtrWrite(), 0,
				sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &numBytesRecv, acceptEvent)
				== false)
			{
				const int32 errCode = ::WSAGetLastError();
				if (errCode != WSA_IO_PENDING)
				{
					RegisterAccept(acceptEvent);
				}
			}
		}

		void Listener::ProcessAccept(AcceptEvent* acceptEvent)
		{
			SPtr<ServerService> serverService = m_owner.lock();
			assert(serverService != nullptr);

			if (serverService->GetNumCurSessions() == serverService->GetNumMaxSessions())
			{
				RegisterAccept(acceptEvent);
				return;
			}

			SPtr<Session> session = std::static_pointer_cast<Session>(acceptEvent->GetHandler());

			if (SetUpdateAcceptSocket(session->GetSocket(), m_socket) == false)
			{
				RegisterAccept(acceptEvent);
				return;
			}

			SOCKADDR_IN sockAddr;
			int32 sizeSockAddr = sizeof(sockaddr);

			if (::getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&sockAddr), &sizeSockAddr)
				== SOCKET_ERROR)
			{
				RegisterAccept(acceptEvent);
				return;
			}

			session->SetAddress(SockaddrWrapper(sockAddr));
			session->ProcessConnect();
			RegisterAccept(acceptEvent);
		}
	}
}
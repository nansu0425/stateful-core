// lib-stateful-core/network/Service.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/network/Service.hpp>
#include <lib-stateful-core/network/Listener.hpp>

namespace StatefulCore
{
	namespace Network
	{
		Service::Service(
			ServiceType type, SockaddrWrapper address, SessionFactory factory, 
			SPtr<IocpEventForwarder> forwarder, int32 numMaxSessions)
			: m_type(type), m_address(address), m_sessionFactory(factory), 
			m_eventForwarder(forwarder), m_numMaxSessions(numMaxSessions)
		{
		}

		void Service::Broadcast(SPtr<SendBufferChunk> sendBufChunk)
		{
			W_SPIN_LOCK;
			
			for (const auto& session : m_sessions)
			{
				session->Send(sendBufChunk);
			}
		}

		SPtr<Session> Service::CreateSession()
		{
			SPtr<Session> session = m_sessionFactory();
			session->SetOwner(shared_from_this());

			if (session == nullptr)
				return nullptr;

			if (m_eventForwarder->Associate(session) == false)
				return nullptr;

			return session;
		}

		void Service::InsertSession(SPtr<Session> session)
		{
			W_SPIN_LOCK;

			m_sessions.insert(session);
			m_numCurSessions++;
		}

		void Service::EraseSession(SPtr<Session> session)
		{
			W_SPIN_LOCK;

			uint64 elemErased = m_sessions.erase(session);
			assert(elemErased > 0);
			m_numCurSessions--;
		}

		ServerService::ServerService(
			SockaddrWrapper localAddr, SessionFactory factory,
			SPtr<IocpEventForwarder> forwarder, int32 numMaxSessions)
			: Service(ServiceType::SERVER, localAddr, factory, forwarder, numMaxSessions)
		{
		}

		bool ServerService::Launch()
		{
			if (CanLaunch() == false)
				return false;

			m_listener = Memory::MakeShared<Listener>();
			if (m_listener == nullptr)
				return false;

			SPtr<ServerService> service = std::static_pointer_cast<ServerService>(shared_from_this());
			if (m_listener->StartListen(service) == false)
				return false;

			return true;
		}

		void ServerService::Terminate()
		{
			Service::Terminate();
		}

		ClientService::ClientService(
			SockaddrWrapper remoteAddr, SessionFactory factory, 
			SPtr<IocpEventForwarder> forwarder, int32 numMaxSessions)
			: Service(ServiceType::CLIENT, remoteAddr, factory, forwarder, numMaxSessions)
		{
		}

		bool ClientService::Launch()
		{
			if (CanLaunch() == false)
				return false;

			const int32 numMaxSessions = m_numMaxSessions;
			for (int32 i = 0; i < numMaxSessions; i++)
			{
				SPtr<Session> session = CreateSession();
				if (session->Connect() == false)
				{
					return false;
				}
			}

			return true;
		}

		void ClientService::Terminate()
		{
			Service::Terminate();
		}
	}
}

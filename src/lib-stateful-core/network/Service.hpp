// lib-stateful-core/network/Service.hpp

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		enum class ServiceType : byte
		{
			SERVER,
			CLIENT,
		};

		using SessionFactory = std::function<SPtr<Session>(void)>;

		/*---------------*
		 *    Service    *
		 *---------------*/

		class Service : public std::enable_shared_from_this<Service>
		{
		public:
			Service(
				ServiceType type, SockaddrWrapper address, SessionFactory factory, 
				SPtr<IocpEventForwarder> forwarder, int32 numMaxSessions = 1);
			virtual ~Service() { }

		public:
			virtual bool	 Launch() abstract;
			bool			 CanLaunch() { return m_sessionFactory != nullptr; }
			virtual void	 Terminate() { };

			void             Broadcast(SPtr<SendBufferChunk> sendBufChunk);
			SPtr<Session>    CreateSession();
			void             InsertSession(SPtr<Session> session);
			void             EraseSession(SPtr<Session> session);

			ServiceType                 GetType() { return m_type; }
			SockaddrWrapper             GetAddress() { return m_address; }
			SPtr<IocpEventForwarder>    GetIocpEventForwarder() { return m_eventForwarder; }
			int32    GetNumCurSessions() { return m_numCurSessions; }
			int32    GetNumMaxSessions() { return m_numMaxSessions; }
			
			void     SetSessionFactory(SessionFactory factory) { m_sessionFactory = factory; }

		protected:
			USE_RW_SPIN_LOCK;
			ServiceType         m_type;
			SockaddrWrapper     m_address = {};
			SessionFactory      m_sessionFactory;
			SPtr<IocpEventForwarder>          m_eventForwarder;
			Memory::HashSet<SPtr<Session>>    m_sessions;
			int32    m_numCurSessions = 0;
			int32    m_numMaxSessions = 0;
		};

		class Listener;

		/*---------------------*
		 *    ServerService    *
		 *---------------------*/

		class ServerService : public Service
		{
		private:
			SPtr<Listener>    m_listener = nullptr;

		public:
			ServerService(SockaddrWrapper localAddr, SessionFactory factory,
				SPtr<IocpEventForwarder> forwarder, int32 numMaxSessions = 1);
			virtual	~ServerService() {}

		public:
			virtual bool    Launch() override;
			virtual void    Terminate() override;
		};

		/*---------------------*
		 *    ClientService    *
		 *---------------------*/

		class ClientService : public Service
		{
		public:
			ClientService(SockaddrWrapper remoteAddr, SessionFactory factory,
				SPtr<IocpEventForwarder> forwarder, int32 numMaxSessions = 1);
			virtual ~ClientService() {}

		public:
			virtual bool    Launch() override;
			virtual void    Terminate() override;
		};
	}
}

// lib-stateful-core/network/IocpEventForwarder.hpp

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		enum class IocpEventType : byte;
		class IocpEventHandler;

		/*-----------------*
		 *    IocpEvent    *
		 *-----------------*/

		class IocpEvent : public OVERLAPPED
		{
		public:
			IocpEvent(IocpEventType type) : m_type(type) { Init(); }

		public:
			void Init();

			IocpEventType             GetType() { return m_type; }
			SPtr<IocpEventHandler>    GetHandler() { return m_handler; }

			void    SetHandler(SPtr<IocpEventHandler> handler) { m_handler = handler; }

		private:
			IocpEventType             m_type;
			SPtr<IocpEventHandler>    m_handler;
		};

		/*------------------------*
		 *    IocpEventHandler    *
		 *------------------------*/

		class IocpEventHandler : public std::enable_shared_from_this<IocpEventHandler>
		{
		public:
			virtual HANDLE    GetHandle() abstract;
			virtual void      Handle(IocpEvent* target, int32 numBytesTransferred = 0) abstract;
		};

		/*--------------------------*
		 *    IocpEventForwarder    *
		 *--------------------------*/

		class IocpEventForwarder
		{
		public:
			IocpEventForwarder();
			~IocpEventForwarder();

		public:
			bool    Associate(SPtr<IocpEventHandler> handler);
			bool    ForwardEvent2Handler(uint32 timeoutMs = INFINITE);

			HANDLE    GetIocp() { return m_iocp; }

		private:
			HANDLE    m_iocp;
		};
	}
}

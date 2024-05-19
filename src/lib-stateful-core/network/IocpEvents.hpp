// lib-stateful-core/network/IocpEvents.hpp

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		class Session;
		class SendBuffer;

		enum class IocpEventType : byte
		{
			CONNECT,
			DISCONNECT,
			ACCEPT,
			RECEIVE,
			SEND,
		};

		class ConnectEvent : public IocpEvent
		{
		public:
			ConnectEvent() : IocpEvent(IocpEventType::CONNECT) { }
		};

		class DisconnectEvent : public IocpEvent
		{
		public:
			DisconnectEvent() : IocpEvent(IocpEventType::DISCONNECT) { }
		};

		class AcceptEvent : public IocpEvent
		{
		public:
			AcceptEvent() : IocpEvent(IocpEventType::ACCEPT) { }

		private:
			SPtr<Session>    m_session = nullptr;
		};

		class ReceiveEvent : public IocpEvent
		{
		public:
			ReceiveEvent() : IocpEvent(IocpEventType::RECEIVE) { }
		};

		class SendEvent : public IocpEvent
		{
		public:
			SendEvent() : IocpEvent(IocpEventType::SEND) { }

		private:
			Memory::Vector<SPtr<SendBuffer>>     m_sendBufs;
		};
	}
}

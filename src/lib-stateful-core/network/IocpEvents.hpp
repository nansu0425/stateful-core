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

		/*--------------------*
		 *    ConnectEvent    *
		 *--------------------*/

		class ConnectEvent : public IocpEvent
		{
		public:
			ConnectEvent() : IocpEvent(IocpEventType::CONNECT) { }
		};

		/*-----------------------*
		 *    DisconnectEvent    *
		 *-----------------------*/

		class DisconnectEvent : public IocpEvent
		{
		public:
			DisconnectEvent() : IocpEvent(IocpEventType::DISCONNECT) { }
		};

		/*-------------------*
		 *    AcceptEvent    *
		 *-------------------*/

		class AcceptEvent : public IocpEvent
		{
		public:
			AcceptEvent() : IocpEvent(IocpEventType::ACCEPT) { }

		public:
			SPtr<Session>    GetSession() { return m_session; }
			void             SetSession(SPtr<Session> session) { m_session = session; }

		private:
			SPtr<Session>    m_session = nullptr;
		};

		/*--------------------*
		 *    ReceiveEvent    *
		 *--------------------*/

		class ReceiveEvent : public IocpEvent
		{
		public:
			ReceiveEvent() : IocpEvent(IocpEventType::RECEIVE) { }
		};

		/*-----------------*
		 *    SendEvent    *
		 *-----------------*/

		class SendEvent : public IocpEvent
		{
		private:
			using VecSendBufChunk = Memory::Vector<SPtr<SendBufferChunk>>;

		public:
			SendEvent() : IocpEvent(IocpEventType::SEND) { }

		public:
			VecSendBufChunk& RefSendBufChunks() { return m_sendBufChunks; }

		private:
			VecSendBufChunk    m_sendBufChunks;
		};
	}
}

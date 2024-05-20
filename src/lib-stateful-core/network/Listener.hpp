// lib-stateful-core/network/Listener.hpp

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		class ServerService;

		/*----------------*
		 *    Listener    *
		 *----------------*/

		class Listener : public IocpEventHandler
		{
		private:
			using VecAcceptEvt = Memory::Vector<AcceptEvent*>;
		
		public:
			~Listener();

		public:
			bool              StartListen(SPtr<ServerService> serverService);
			void              CloseSocket();
			virtual void      Handle(IocpEvent* target, int32 numBytesTransferred = 0) override;

			virtual HANDLE    GetHandle() override;

		private:
			void RegisterAccept(AcceptEvent* acceptEvent);
			void ProcessAccept(AcceptEvent* acceptEvent);
		
		private:
			SOCKET                 m_socket = INVALID_SOCKET;
			VecAcceptEvt           m_acceptEvents;
			WPtr<ServerService>    m_owner;
		};
	}
}

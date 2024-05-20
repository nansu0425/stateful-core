// lib-stateful-core/network/Session.hpp

#pragma once

#include <lib-stateful-core/network/IocpEvents.hpp>

namespace StatefulCore
{
	namespace Network
	{
		class Service;

		/*---------------*
		 *    Session    *
		 *---------------*/

		class Session : public IocpEventHandler
		{
		private:
			friend class Listener;
			friend class IocpEventForwarder;
			friend class Service;

			enum { BUF_SIZE = 0x10000 };

		public:
			Session();
			virtual	~Session();

		public:
			virtual void    Handle(IocpEvent* target, int32 numBytesTransferred = 0) override;
			void		    Handle(int32 errCode);
			bool		    Connect();
			bool            IsConnected() { return m_connected; }
			void		    Disconnect(const WCHAR* cause);
			void		    Send(SPtr<SendBufferChunk> sendBufChunk);

			SPtr<Session>      GetShared() { return std::static_pointer_cast<Session>(shared_from_this()); }
			SPtr<Service>      GetOwner() { return m_owner.lock(); }
			SOCKET             GetSocket() { return m_socket; }
			SockaddrWrapper    GetAddress() { return m_address; }
			virtual HANDLE	   GetHandle() override;

			void    SetAddress(SockaddrWrapper address) { m_address = address; }
			void    SetOwner(SPtr<Service> owner) { m_owner = owner; }

		protected:
			virtual void	ProcessConnect() { }
			virtual void	ProcessDisconnect() { }
			virtual int32	ProcessReceive(byte* buf, int32 numBytesRecv) { return numBytesRecv; }
			virtual void	ProcessSend(int32 numBytesSent) { }

		private:
			bool    RegisterConnect();
			bool    RegisterDisconnect();
			void    RegisterReceive();
			void    RegisterSend();

			void    HandleConnect();
			void    HandleDisconnect();
			void    HandleReceive(int32 numBytesRecv);
			void    HandleSend(int32 numBytesSent);

		private:
			SOCKET             m_socket = INVALID_SOCKET;
			SockaddrWrapper    m_address = {};
			Atomic<bool>       m_connected = false;
			WPtr<Service>      m_owner;

			USE_RW_SPIN_LOCK;
			ReceiveBuffer      m_recvBuf;
			Memory::Queue<SPtr<SendBufferChunk>>    m_sendQueue;
			Atomic<bool>       m_sendRegistered = false;

			ReceiveEvent       m_recvEvent;
			SendEvent          m_sendEvent;
			ConnectEvent       m_connEvent;
			DisconnectEvent    m_disconnEvent;
		};

		/*--------------------*
		 *    PacketHeader    *
		 *--------------------*/

		struct PacketHeader
		{
			uint16 size;
			uint16 type;
		};

		/*---------------------*
		 *    PacketSession    *
		 *---------------------*/

		class PacketSession : public Session
		{
		public:
			virtual ~PacketSession() { };

			SPtr<PacketSession> GetShared()
			{
				return std::static_pointer_cast<PacketSession>(shared_from_this());
			}

		protected:
			virtual int32	ProcessReceive(byte* buf, int32 numBytesRecv) final;
			virtual void	ProcessPacketRecv(byte* buf, int32 packetSize) abstract;
		};
	}
}

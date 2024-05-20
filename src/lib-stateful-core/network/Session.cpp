// lib-stateful-core/network/Session.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/network/Session.hpp>
#include <lib-stateful-core/network/SocketFunctions.hpp>
#include <lib-stateful-core/network/Service.hpp>

namespace StatefulCore
{
	namespace Network
	{
		Session::Session() : m_recvBuf(BUF_SIZE)
		{
			m_socket = CreateSocket();
		}

		Session::~Session()
		{
			Close(m_socket);
		}

		void Session::Handle(IocpEvent* target, int32 numBytesTransferred)
		{
			switch (target->GetType())
			{
			case IocpEventType::CONNECT:
				HandleConnect();
				break;
			case IocpEventType::DISCONNECT:
				HandleDisconnect();
				break;
			case IocpEventType::RECEIVE:
				HandleReceive(numBytesTransferred);
				break;
			case IocpEventType::SEND:
				HandleSend(numBytesTransferred);
				break;
			default:
				break;
			}
		}

		void Session::Handle(int32 errCode)
		{
			switch (errCode)
			{
			case WSAECONNRESET:
			case WSAECONNABORTED:
				Disconnect(L"HadleError");
				break;
			default:
				std::cout << "errCode: " << errCode << std::endl;
				break;
			}
		}

		bool Session::Connect()
		{
			return RegisterConnect();
		}

		void Session::Disconnect(const WCHAR* cause)
		{
			if (m_connected.exchange(false) == false)
				return;

			std::wcout << cause << std::endl;

			RegisterDisconnect();
		}

		void Session::Send(SPtr<SendBufferChunk> sendBufChunk)
		{
			if (IsConnected() == false)
				return;

			bool registered = true;

			{
				W_SPIN_LOCK;

				m_sendQueue.push(sendBufChunk);

				if (m_sendRegistered.exchange(true) == false)
					registered = false;
			}

			if (!registered)
				RegisterSend();
		}

		HANDLE Session::GetHandle()
		{
			return reinterpret_cast<HANDLE>(m_socket);
		}

		bool Session::RegisterConnect()
		{
			if (IsConnected() == true)
				return false;

			if (GetOwner()->GetType() != ServiceType::CLIENT)
				return false;

			if (SetReuseAddress(m_socket, true) == false)
				return false;

			if (BindAnyAddress(m_socket, 0) == false)
				return false;

			m_connEvent.Init();
			m_connEvent.SetHandler(shared_from_this());

			SOCKADDR_IN sockAddr = GetOwner()->GetAddress().GetSockaddr();
			DWORD numBytesSent = 0;

			if (ConnectEx(
				m_socket, reinterpret_cast<SOCKADDR*>(&sockAddr),
				sizeof(sockAddr), NULL, 0, &numBytesSent, &m_connEvent)
				== false)
			{
				const int32 errCode = ::WSAGetLastError();
				if (errCode != WSA_IO_PENDING)
				{
					m_connEvent.SetHandler(nullptr);
					Handle(errCode);
					return false;
				}
			}

			return true;
		}

		bool Session::RegisterDisconnect()
		{
			m_disconnEvent.Init();
			m_disconnEvent.SetHandler(shared_from_this());

			if (DisconnectEx(m_socket, &m_disconnEvent, TF_REUSE_SOCKET, 0) == false)
			{
				const int32 errCode = ::WSAGetLastError();
				if (errCode != WSA_IO_PENDING)
				{
					m_disconnEvent.SetHandler(nullptr);
					return false;
				}
			}

			return true;
		}

		void Session::RegisterReceive()
		{
			if (IsConnected() == false)
				return;

			m_recvEvent.Init();
			m_recvEvent.SetHandler(shared_from_this());

			WSABUF wsaBuf{
				static_cast<ULONG>(m_recvBuf.GetFreeSize()),
				reinterpret_cast<CHAR*>(m_recvBuf.GetPtrWrite()) };
			DWORD numBytesRecv = 0;
			DWORD flags = 0;

			if (::WSARecv(
				m_socket, &wsaBuf, 1, &numBytesRecv, &flags, &m_recvEvent, NULL)
				== SOCKET_ERROR)
			{
				const int32 errCode = ::WSAGetLastError();
				if (errCode != WSA_IO_PENDING)
				{
					Handle(errCode);
					m_recvEvent.SetHandler(nullptr);
				}
			}
		}

		void Session::RegisterSend()
		{
			if (IsConnected() == false)
				return;

			m_sendEvent.Init();
			m_sendEvent.SetHandler(GetShared());

			auto& sendBufChunks = m_sendEvent.RefSendBufChunks();

			{
				W_SPIN_LOCK;

				int32 numBytesWritten = 0;

				while (m_sendQueue.empty() == false)
				{
					SPtr<SendBufferChunk> sendBufChunk = m_sendQueue.front();
					m_sendQueue.pop();

					numBytesWritten += sendBufChunk->GetNumBytesWritten();
					sendBufChunks.push_back(sendBufChunk);
				}
			}

			Memory::Vector<WSABUF> wsaBufs;
			wsaBufs.reserve(sendBufChunks.size());

			for (auto sendBufChunk : sendBufChunks)
			{
				WSABUF wsaBuf{
					static_cast<ULONG>(sendBufChunk->GetNumBytesWritten()),
					reinterpret_cast<CHAR*>(sendBufChunk->GetChunk()) };
				wsaBufs.push_back(wsaBuf);
			}

			DWORD numBytesSent = 0;

			if (::WSASend(
				m_socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()),
				&numBytesSent, 0, &m_sendEvent, NULL) == SOCKET_ERROR)
			{
				const int32 errCode = ::WSAGetLastError();
				if (errCode != WSA_IO_PENDING)
				{
					Handle(errCode);
					m_sendEvent.SetHandler(nullptr);
					sendBufChunks.clear();
					m_sendRegistered.store(false);
				}
			}
		}

		void Session::HandleConnect()
		{
			m_connEvent.SetHandler(nullptr);
			m_connected.store(true);

			GetOwner()->InsertSession(GetShared());
			ProcessConnect();
			RegisterReceive();
		}

		void Session::HandleDisconnect()
		{
			m_disconnEvent.SetHandler(nullptr);

			GetOwner()->EraseSession(GetShared());
			ProcessDisconnect();
		}

		void Session::HandleReceive(int32 numBytesRecv)
		{
			m_recvEvent.SetHandler(nullptr);

			if (numBytesRecv == 0)
			{
				Disconnect(L"The size of data received: 0B");
				return;
			}

			if (m_recvBuf.ProcessWrite(numBytesRecv) == false)
			{
				Disconnect(L"Failed to process writing to receive buffer.");
				return;
			}

			int32 dataSize = m_recvBuf.GetDataSize();
			int32 numBytesProcessed = ProcessReceive(m_recvBuf.GetPtrRead(), dataSize);

			if (numBytesProcessed < 0 ||
				dataSize < numBytesProcessed ||
				m_recvBuf.ProcessRead(numBytesProcessed) == false)
			{
				Disconnect(L"Failed to process reading receive buffer.");
				return;
			}

			m_recvBuf.Rearrange();
			RegisterReceive();
		}

		void Session::HandleSend(int32 numBytesSent)
		{
			m_sendEvent.SetHandler(nullptr);
			m_sendEvent.RefSendBufChunks().clear();

			if (numBytesSent == 0)
			{
				Disconnect(L"The size of data sent: 0B");
				return;
			}

			ProcessSend(numBytesSent);

			W_SPIN_LOCK;

			if (m_sendQueue.empty())
				m_sendRegistered.store(false);
			else
				RegisterSend();
		}
		int32 PacketSession::ProcessReceive(byte* buf, int32 numBytesRecv)
		{
			int32 numProcessedBytes = 0;

			while (true)
			{
				int32 numRemainingBytes = numBytesRecv - numProcessedBytes;

				if (numRemainingBytes < sizeof(PacketHeader))
					break;

				PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buf[numProcessedBytes]));

				if (numRemainingBytes < header.size)
					break;

				ProcessPacketRecv(&buf[numProcessedBytes], header.size);
				numProcessedBytes += header.size;
			}

			return numProcessedBytes;
		}
}
}

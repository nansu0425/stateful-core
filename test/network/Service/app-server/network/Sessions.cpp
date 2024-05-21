// app-server/network/Sessions.cpp

#include <app-server/precompiled/Pch.hpp>
#include <app-server/network/Sessions.hpp>
#include <lib-stateful-core/network/SendBufferManager.hpp>

namespace Server
{
	namespace Network
	{
		ClientSessionManager    g_clientSessionManager;

		void ClientSession::ProcessConnect()
		{
			std::cout << "Connected to client!" << std::endl;
			g_clientSessionManager.Insert(std::static_pointer_cast<ClientSession>(shared_from_this()));
		}

		void ClientSession::ProcessDisconnect()
		{
			std::cout << "Disconnected to client..." << std::endl;
			g_clientSessionManager.Erase(std::static_pointer_cast<ClientSession>(shared_from_this()));
		}

		void ClientSession::ProcessPacketRecv(byte* buf, int32 packetSize)
		{
			namespace CoreNet = StatefulCore::Network;

			CoreNet::PacketHeader header = *(reinterpret_cast<CoreNet::PacketHeader*>(buf));
			std::cout << "Received packet[size=" << header.size 
				<< ", type=" << header.type << "]" << std::endl;

			SPtr<CoreNet::SendBufferChunk> sendBufChunk =
				CoreNet::g_sendBufferManager->StartWritingChunk(4096);
			byte* chunk = sendBufChunk->GetChunk();

			::memcpy_s(chunk, 4096, buf, packetSize);
			sendBufChunk->EndWriting(packetSize);

			g_clientSessionManager.Broadcast(sendBufChunk);
		}

		void ClientSession::ProcessSend(int32 numBytesSent)
		{
			std::cout << "Sent " << numBytesSent << "B" << std::endl;
		}

		void ClientSessionManager::Insert(SPtr<ClientSession> session)
		{
			W_SPIN_LOCK;
			m_sessions.insert(session);
		}

		void ClientSessionManager::Erase(SPtr<ClientSession> session)
		{
			W_SPIN_LOCK;
			m_sessions.erase(session);
		}

		void ClientSessionManager::Broadcast(SPtr<SendBufChunk> sendBufChunk)
		{
			W_SPIN_LOCK;

			for (SPtr<ClientSession> session : m_sessions)
				session->Send(sendBufChunk);
		}
	}
}

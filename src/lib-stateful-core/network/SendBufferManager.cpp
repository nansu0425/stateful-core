// lib-stateful-core/network/SendBufferManager.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/network/SendBufferManager.hpp>

namespace StatefulCore
{
	namespace Network
	{
		void SendBufferChunk::EndWriting(uint32 numBytesWritten)
		{
			assert(m_size >= numBytesWritten);
			m_numBytesWritten = numBytesWritten;
			m_owner->EndWrittingChunk(numBytesWritten);
		}

		void SendBuffer::Init()
		{
			m_writing = false;
			m_idxWrite = 0;
		}

		SPtr<SendBufferChunk> SendBuffer::StartWritingChunk(uint32 size)
		{
			assert(size <= BUF_SIZE);
			assert(m_writing == false);

			if (size > GetFreeSize())
				return nullptr;

			m_writing = true;

			return Memory::ObjectPool<SendBufferChunk>::MakeShared(
				shared_from_this(), GetFreeBuffer(), size);
		}

		void SendBuffer::EndWrittingChunk(uint32 numBytesWritten)
		{
			assert(m_writing == true);
			m_writing = false;
			m_idxWrite += numBytesWritten;
		}

		SPtr<SendBufferChunk> SendBufferManager::StartWritingChunk(uint32 size)
		{
			if (l_sendBuf == nullptr)
			{
				l_sendBuf = Pop();
				l_sendBuf->Init();
			}

			assert(l_sendBuf->IsWriting() == false);

			if (l_sendBuf->GetFreeSize() < size)
			{
				l_sendBuf = Pop();
				l_sendBuf->Init();
			}

			return l_sendBuf->StartWritingChunk(size);
		}

		SPtr<SendBuffer> SendBufferManager::Pop()
		{
			{
				W_SPIN_LOCK;

				if (m_sendBufs.empty() == false)
				{
					SPtr<SendBuffer> sendBuf = m_sendBufs.back();
					m_sendBufs.pop_back();

					return sendBuf;
				}
			}

			return SPtr<SendBuffer>(Memory::New<SendBuffer>(), Network::Push);
		}

		void SendBufferManager::Push(SPtr<SendBuffer> sendBuf)
		{
			W_SPIN_LOCK;
			m_sendBufs.push_back(sendBuf);
		}

		void Push(SendBuffer* sendBuf)
		{
			g_sendBufferManager->Push(SPtr<SendBuffer>(sendBuf, Push));
		}
	}
}
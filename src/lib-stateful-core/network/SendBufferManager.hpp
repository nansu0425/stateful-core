// lib-stateful-core/network/SendBufferManager.hpp

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		class SendBuffer;

		/*-----------------------*
		 *    SendBufferChunk    *
		 *-----------------------*/

		class SendBufferChunk : public std::enable_shared_from_this<SendBufferChunk>
		{
		public:
			SendBufferChunk(SPtr<SendBuffer> owner, byte* chunk, uint32 size)
				:m_owner(owner), m_chunk(chunk), m_size(size) { }

		public:
			byte*     GetChunk() { return m_chunk; }
			uint32    GetSize() { return m_size; }
			uint32    GetNumBytesWritten() { return m_numBytesWritten; }
			void      EndWriting (uint32 numBytesWritten);

		private:
			SPtr<SendBuffer>    m_owner;
			byte*     m_chunk;
			uint32    m_size = 0;
			uint32    m_numBytesWritten = 0;
		};

		/*------------------*
		 *    SendBuffer    *
		 *------------------*/

		class SendBuffer : public std::enable_shared_from_this<SendBuffer>
		{
		private:
			enum { BUF_SIZE = 6000 };

		public:
			void                     Init();
			SPtr<SendBufferChunk>    StartWritingChunk(uint32 size);
			void                     EndWrittingChunk(uint32 numBytesWritten);

			bool      IsWriting() { return m_writing; }
			byte*     GetFreeBuffer() { return &m_buffer[m_idxWrite]; }
			uint32    GetFreeSize() { return static_cast<uint32>(m_buffer.size() - m_idxWrite); }

		private:
			Memory::Array<byte, BUF_SIZE>    m_buffer;
			bool      m_writing;
			uint32    m_idxWrite = 0;
		};

		/*-------------------------*
		 *    SendBufferManager    *
		 *-------------------------*/

		class SendBufferManager
		{
		public:
			SPtr<SendBufferChunk>    StartWritingChunk(uint32 size);

			SPtr<SendBuffer>    Pop();
			void                Push(SPtr<SendBuffer> sendBuf);
		
		private:
			USE_RW_SPIN_LOCK;
			Memory::Vector<SPtr<SendBuffer>> m_sendBufs;
		};

		void Push(SendBuffer* sendBuf);
	}
}

// lib-stateful-core/network/ReceiveBuffer.hpp

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		/*---------------------*
		 *    ReceiveBuffer    *
		 *---------------------*/

		class ReceiveBuffer
		{
		private:
			enum { SIZE_MULTIPLE = 10 };

		public:
			ReceiveBuffer(int32 size);

		public:
			void    Rearrange();

			byte*    GetIndexRead() { return &m_buffer[m_idxRead]; }
			byte*    GetIndexWrite() { return &m_buffer[m_idxWrite]; }
			int32    GetDataSize() { return m_idxWrite - m_idxRead; }
			int32    GetFreeSize() { return m_capacity - m_idxWrite; }

			bool    ProcessRead(int32 numBytesRead);
			bool    ProcessWrite(int32 numBytesWritten);

		private:
			int32    m_capacity = 0;
			int32    m_size = 0;
			int32    m_idxRead = 0;
			int32    m_idxWrite = 0;
			Memory::Vector<byte>    m_buffer;
		};
	}
}

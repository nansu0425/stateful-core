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

			byte*    GetPosRead() { return &m_buffer[m_posRead]; }
			byte*    GetPosWrite() { return &m_buffer[m_posWrite]; }
			int32    GetDataSize() { return m_posWrite - m_posRead; }
			int32    GetFreeSize() { return m_capacity - m_posWrite; }

			bool    SetPosRead(int32 size);
			bool    SetPosWrite(int32 size);

		private:
			int32                   m_capacity = 0;
			int32                   m_size = 0;
			int32                   m_posRead = 0;
			int32                   m_posWrite = 0;
			Memory::Vector<byte>    m_buffer;
		};
	}
}

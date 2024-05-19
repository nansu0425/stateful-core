// lib-stateful-core/network/ReceiveBuffer.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/network/ReceiveBuffer.hpp>

namespace StatefulCore
{
	namespace Network
	{
		ReceiveBuffer::ReceiveBuffer(int32 size)
		{
			m_capacity = size * SIZE_MULTIPLE;
			m_buffer.resize(m_capacity);
		}

		void ReceiveBuffer::Rearrange()
		{
			int32 dataSize = GetDataSize();

			if (dataSize == 0)
				m_posRead = m_posWrite = 0;
			else if (GetFreeSize() < m_size)
			{
				::memcpy_s(&m_buffer[0], dataSize, &m_buffer[m_posRead], dataSize);
				m_posRead = 0;
				m_posWrite = dataSize;
			}
		}

		bool ReceiveBuffer::SetPosRead(int32 size)
		{
			if (size > GetDataSize())
				return false;

			m_posRead += size;

			return true;
		}

		bool ReceiveBuffer::SetPosWrite(int32 size)
		{
			if (size > GetFreeSize())
				return false;

			m_posWrite += size;

			return true;
		}
	}
}

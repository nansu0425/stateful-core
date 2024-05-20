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
				m_idxRead = m_idxWrite = 0;
			else if (GetFreeSize() < m_size)
			{
				::memcpy_s(&m_buffer[0], dataSize, &m_buffer[m_idxRead], dataSize);
				m_idxWrite = 0;
				m_idxRead = dataSize;
			}
		}

		bool ReceiveBuffer::ProcessRead(int32 numBytesRead)
		{
			if (numBytesRead > GetDataSize())
				return false;

			m_idxRead += numBytesRead;

			return true;
		}

		bool ReceiveBuffer::ProcessWrite(int32 numBytesWritten)
		{
			if (numBytesWritten > GetFreeSize())
				return false;

			m_idxWrite += numBytesWritten;

			return true;
		}
	}
}

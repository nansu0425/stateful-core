// lib-stateful-core/network/IocpEventForwarder.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/network/IocpEventForwarder.hpp>

namespace StatefulCore
{
	namespace Network
	{
		void IocpEvent::Init()
		{
			OVERLAPPED::Internal = 0;
			OVERLAPPED::InternalHigh = 0;
			OVERLAPPED::Pointer = 0;
			OVERLAPPED::hEvent = 0;
		}

		IocpEventForwarder::IocpEventForwarder()
		{
			m_iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
			assert(m_iocp != INVALID_HANDLE_VALUE);
		}
		
		IocpEventForwarder::~IocpEventForwarder()
		{
			::CloseHandle(m_iocp);
		}

		bool IocpEventForwarder::Associate(SPtr<IocpEventHandler> handler)
		{
			return ::CreateIoCompletionPort(handler->GetHandle(), m_iocp, 0, 0);
		}

		bool IocpEventForwarder::ForwardEvent2Handler(uint32 timeoutMs)
		{
			DWORD numBytesTransferred = 0;
			ULONG_PTR key = 0;
			IocpEvent* passedEvent = nullptr;

			if (::GetQueuedCompletionStatus(
				m_iocp, 
				OUT &numBytesTransferred,
				OUT &key, 
				OUT reinterpret_cast<LPOVERLAPPED*>(&passedEvent),
				timeoutMs))
			{
				SPtr<IocpEventHandler> handler = passedEvent->GetHandler();
				handler->Handle(passedEvent, numBytesTransferred);
			}
			else
			{
				int32 errCode = ::WSAGetLastError();

				switch (errCode)
				{
				case WAIT_TIMEOUT:
					return false;
				default:
					SPtr<IocpEventHandler> handler = passedEvent->GetHandler();
					handler->Handle(passedEvent, numBytesTransferred);
					break;
				}
			}

			return true;
		}
	}
}

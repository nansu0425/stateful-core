// lib-stateful-core/network/IocpEvents.hpp

#pragma once

namespace StatefulCore
{
	namespace Network
	{
		enum class IocpEventType : byte
		{
			CONNECT,
			DISCONNECT,
			ACCEPT,
			RECEIVE,
			SEND,
		};


	}
}

// lib-stateful-core/common/Types.hpp

#pragma once

namespace StatefulCore
{
	// Byte
	using byte = unsigned char;

	// Integer
	using int16 = __int16;
	using int32 = __int32;
	using int64 = __int64;
	using uint16 = unsigned __int16;
	using uint32 = unsigned __int32;
	using uint64 = unsigned __int64;

	// Multithreading
	using Thread = std::thread;
	using ThreadId = uint32;
	using VecThread = std::vector<Thread>;
	template<typename T>
	using Atomic = std::atomic<T>;
	using Mutex = std::mutex;
	using CondVar = std::condition_variable;
	using UniLock = std::unique_lock<Mutex>;
	using LockGrd = std::lock_guard<Mutex>;

	// Utilities
	template<typename FuncType>
	using Func = std::function<FuncType>;
}

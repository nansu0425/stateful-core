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

	// Containers
	template<typename T>
	using Vec = std::vector<T>;
	template<typename KeyT, typename ValT>
	using TreeMap = std::map<KeyT, ValT>;
	template<typename KeyT, typename ValT>
	using HashMap = std::unordered_map<KeyT, ValT>;
	template<typename T>
	using TreeSet = std::set<T>;
	template<typename T>
	using HashSet = std::unordered_set<T>;

	// Multithreading
	using Thread = std::thread;
	using ThreadId = uint16;
	template<typename T>
	using Atomic = std::atomic<T>;
	using Mutex = std::mutex;
	using CondVar = std::condition_variable;
	using UniLock = std::unique_lock<Mutex>;
	using LockGrd = std::lock_guard<Mutex>;
	using LockName = const char*;
	using LockId = int32;

	// Utilities
	template<typename FuncT>
	using Func = std::function<FuncT>;
}

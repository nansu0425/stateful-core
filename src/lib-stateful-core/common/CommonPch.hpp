// lib-stateful-core/common/CommonPch.hpp

#pragma once

/*--------------------*
 *    C++ Standard    *
 *--------------------*/

// Input/output
#include <iostream>

// Containers
#include <array>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

// Thread support
#include <mutex>

// Atomic Operations
#include <atomic>

// Utilities
#include <functional>
#include <chrono>

// Error handling
#include <cassert>

// Dynamic memory management
#include <memory>

/*---------------*
 *    Windows    *
 *---------------*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Windows Sockets 2
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "Ws2_32.lib")

 /*---------------------*
  *    Stateful Core    *
  *---------------------*/

// Common
#include <lib-stateful-core/common/Macros.hpp>
#include <lib-stateful-core/common/Types.hpp>
#include <lib-stateful-core/common/ThreadLocalStorage.hpp>
#include <lib-stateful-core/common/Global.hpp>

// Multithreading
#include <lib-stateful-core/multithreading/RwLock.hpp>

// Memory
#include <lib-stateful-core/memory/Allocator.hpp>
#include <lib-stateful-core/memory/MemoryManager.hpp>
#include <lib-stateful-core/memory/ObjectPool.hpp>
#include <lib-stateful-core/memory/Containers.hpp>

// Network
#include <lib-stateful-core/network/IocpEventForwarder.hpp>
#include <lib-stateful-core/network/SockaddrWrapper.hpp>
#include <lib-stateful-core/network/ReceiveBuffer.hpp>
#include <lib-stateful-core/network/SendBufferManager.hpp>
#include <lib-stateful-core/network/Session.hpp>

// Asynchronous
#include <lib-stateful-core/asynchronous/JobQueue.hpp>

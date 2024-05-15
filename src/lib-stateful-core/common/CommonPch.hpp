// lib-stateful-core/common/CommonPch.hpp

#pragma once

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

// Multithreading
#include <mutex>
#include <atomic>

// Utilities
#include <functional>
#include <cassert>

// Windows
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

#pragma comment(lib, "Ws2_32.lib")

// Common
#include <lib-stateful-core/common/Macros.hpp>
#include <lib-stateful-core/common/Types.hpp>
#include <lib-stateful-core/common/ThreadLocalStorage.hpp>
#include <lib-stateful-core/common/Global.hpp>

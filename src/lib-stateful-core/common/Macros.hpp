// lib-stateful-core/common/Macros.hpp

#pragma once

#define OUT

#define SIZE_16(val)     static_cast<int16>(sizeof(val))
#define SIZE_32(val)     static_cast<int32>(sizeof(val))
#define LENTH_16(arr)    static_cast<int16>(sizeof(arr)/sizeof(arr[0]))
#define LENTH_32(arr)    static_cast<int32>(sizeof(arr)/sizeof(arr[0]))

/*----------------------*
 *    Multithreading    *
 *----------------------*/

// Readers-Writer Spin Lock
#define USE_RW_SPIN_LOCKS(count)    StatefulCore::Multithreading::RwSpinLock locks[count]
#define USE_RW_SPIN_LOCK            USE_RW_SPIN_LOCKS(1)
#define R_SPIN_LOCK_IDX(idx)        StatefulCore::Multithreading::RLockGuard rGuard##idx(locks[idx], typeid(this).name())
#define R_SPIN_LOCK                 R_SPIN_LOCK_IDX(0)
#define W_SPIN_LOCK_IDX(idx)        StatefulCore::Multithreading::WLockGuard wGuard##idx(locks[idx], typeid(this).name())
#define W_SPIN_LOCK                 W_SPIN_LOCK_IDX(0)

/*--------------*
 *    Memory    *
 *--------------*/

// Stomp allocator option
#define _STOMP

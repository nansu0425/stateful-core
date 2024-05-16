// lib-stateful-core/common/Macros.hpp

#pragma once

#define OUT

// Readers-Writer Spin Lock
#define USE_RW_SPIN_LOCKS(count)    StatefulCore::Multithreading::RwSpinLock locks[count]
#define USE_RW_SPIN_LOCK            USE_RW_SPIN_LOCKS(1)
#define R_SPIN_LOCK_IDX(idx)        StatefulCore::Multithreading::RLockGuard rGuard##idx(locks[idx], typeid(this).name())
#define R_SPIN_LOCK                 R_SPIN_LOCK_IDX(0)
#define W_SPIN_LOCK_IDX(idx)        StatefulCore::Multithreading::WLockGuard wGuard##idx(locks[idx], typeid(this).name())
#define W_SPIN_LOCK                 W_SPIN_LOCK_IDX(0)

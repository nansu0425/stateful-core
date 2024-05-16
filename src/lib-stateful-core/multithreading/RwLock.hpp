// lib-stateful-core/multithreading/RwLock.hpp

#pragma once

namespace StatefulCore
{
	namespace Multithreading
	{
		/*--------------*
		 *    RwLock    *
		 *--------------*/
		class RwLock
		{
		protected:
			// [Writer thread id (16bit)][Readers count (16bit)]
			using LockFlag = uint32;

			enum LockFlagMask : LockFlag
			{
				WRITER_THREAD_MASK    = 0xFFFF'0000,
				READERS_COUNT_MASK    = 0x0000'FFFF,
				EMPTY_MASK            = 0x0000'0000,
			};

		public:
			virtual void AcquireR(LockName name) abstract;
			virtual void AcquireW(LockName name) abstract;
			virtual void ReleaseR(LockName name) abstract;
			virtual void ReleaseW(LockName name) abstract;

		protected:
			Atomic<LockFlag> m_lockFlag = LockFlagMask::EMPTY_MASK;
			int16 m_acquiredWCount = 0;
		};

		/*------------------*
		 *    RwSpinLock    *
		 *------------------*/
		class RwSpinLock : public RwLock
		{
		private:
			enum SpinConst : uint32
			{
				ACQUIRE_TIMEOUT_TICK = 10000,
				MAX_SPIN_COUNT = 5000,
			};

		public:
			virtual void AcquireR(LockName name) override;
			virtual void AcquireW(LockName name) override;
			virtual void ReleaseR(LockName name) override;
			virtual void ReleaseW(LockName name) override;
		};

		/*------------------*
		 *    RLockGuard    *
		 *------------------*/
		class RLockGuard
		{
		public:
			RLockGuard(RwLock& lock, LockName name)
				: m_lock(lock), m_name(name)
			{
				m_lock.AcquireR(m_name);
			}

			~RLockGuard()
			{
				m_lock.ReleaseR(m_name);
			}

		private:
			RwLock&     m_lock;
			LockName    m_name;
		};

		/*------------------*
		 *    WLockGuard    *
		 *------------------*/
		class WLockGuard
		{
		public:
			WLockGuard(RwLock& lock, LockName name)
				: m_lock(lock), m_name(name)
			{
				m_lock.AcquireW(m_name);
			}

			~WLockGuard()
			{
				m_lock.ReleaseW(m_name);
			}

		private:
			RwLock&     m_lock;
			LockName    m_name;
		};
	}
}

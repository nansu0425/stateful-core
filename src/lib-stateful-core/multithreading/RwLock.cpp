// lib-stateful-core/multithreading/RwLock.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/multithreading/RwLock.hpp>

namespace StatefulCore
{
	namespace Multithreading
	{
		void RwSpinLock::AcquireR(const char* name)
		{
			const ThreadId writerId = m_lockFlag.load() >> 16;

			// Acquire readers lock after acquiring writer lock.
			if (writerId == l_threadId)
			{
				m_lockFlag.fetch_add(1);
				return;
			}

			const uint64 beginTick = ::GetTickCount64();

			// Acquire readers lock.
			while (true)
			{
				for (uint32 spinCnt = 0; spinCnt < SpinConst::MAX_SPIN_COUNT; spinCnt++)
				{
					LockFlag expected = m_lockFlag.load() & LockFlagMask::READERS_COUNT_MASK;

					if (m_lockFlag.compare_exchange_weak(OUT expected, expected + 1))
						return;
				}

				// Check spin timeout.
				assert(::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK);
				std::this_thread::yield();
			}
		}

		void RwSpinLock::AcquireW(const char* name)
		{
			const ThreadId writerId = m_lockFlag.load() >> 16;

			// Reacquire writer lock.
			if (writerId == l_threadId)
			{
				m_acquiredWCount++;
				return;
			}

			const LockFlag desired = static_cast<LockFlag>(l_threadId) << 16;
			const uint64 beginTick = ::GetTickCount64();

			// Acquire writer lock.
			while (true)
			{
				for (uint32 spinCnt = 0; spinCnt < SpinConst::MAX_SPIN_COUNT; spinCnt++)
				{
					LockFlag expected = LockFlagMask::EMPTY_MASK;

					if (m_lockFlag.compare_exchange_weak(OUT expected, desired))
					{
						m_acquiredWCount++;
						return;
					}
				}

				// Check spin timeout.
				assert(::GetTickCount64() - beginTick < SpinConst::ACQUIRE_TIMEOUT_TICK);
				std::this_thread::yield();
			}
		}

		void RwSpinLock::ReleaseR(const char* name)
		{
			// Check multiple releasing.
			assert((m_lockFlag.fetch_sub(1) & LockFlagMask::READERS_COUNT_MASK) == 0);
		}

		void RwSpinLock::ReleaseW(const char* name)
		{
			// Check invalid lock oreder.
			assert((m_lockFlag & LockFlagMask::READERS_COUNT_MASK) == 0);

			const int32 lockCnt = --m_acquiredWCount;

			if (lockCnt == 0)
				m_lockFlag.store(LockFlagMask::EMPTY_MASK);
		}
	}
}

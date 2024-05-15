// lib-stateful-core/multithreading/RwLock.hpp

#pragma once

namespace StatefulCore
{
	namespace Multithreading
	{
		/*
		 * Lock Flag (32bit)
		 * [Writer thread id (16bit)][Readers count (16bit)]
		 * 
		 * Writer thread id: exclusive lock owner
		 * Readers count: shared lock count
		 */
		class RwLock
		{
		protected:
			using LockFlag = uint32;

			enum LockFlagMask : LockFlag
			{
				WRITER_THREAD_MASK    = 0xFFFF'0000,
				READERS_COUNT_MASK    = 0x0000'FFFF,
				EMPTY_MASK            = 0x0000'0000,
			};

		public:
			virtual void AcquireR(const char* name = "") abstract;
			virtual void AcquireW(const char* name = "") abstract;
			virtual void ReleaseR(const char* name = "") abstract;
			virtual void ReleaseW(const char* name = "") abstract;

		protected:
			Atomic<LockFlag> m_lockFlag    = LockFlagMask::EMPTY_MASK;
			int16 m_acquiredWCount         = 0;
		};

		class RwSpinLock : public RwLock
		{
		private:
			enum SpinConst : uint32
			{
				ACQUIRE_TIMEOUT_TICK    = 10000,
				MAX_SPIN_COUNT          = 5000,
			};

		public:
			virtual void AcquireR(const char* name = "") override;
			virtual void AcquireW(const char* name = "") override;
			virtual void ReleaseR(const char* name = "") override;
			virtual void ReleaseW(const char* name = "") override;
		};
	}
}

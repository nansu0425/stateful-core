// lib-stateful-core/multithreading/DeadLockProfiler.hpp

#pragma once

namespace StatefulCore
{
	namespace Multithreading
	{
		/*------------------------*
		 *    DeadLockProfiler    *
		 *------------------------*/
		class DeadLockProfiler
		{
		public:
			void PushLock(LockName name);
			void PopLock(LockName name);

		private:
			void CheckCycle();
			void Dfs(LockId id);

		private:
			HashMap<LockName, LockId>           m_nameToId;
			HashMap<LockId, LockName>           m_idToName;
			TreeMap<LockId, TreeSet<LockId>>    m_lockGraph;

			Vec<LockId>    m_discoveredHistory;
			int32          m_discoveredOrder = 0;
			Vec<bool>      m_finished;
			Vec<LockId>    m_parent;

			Mutex    m_lock;
		};

	}
}

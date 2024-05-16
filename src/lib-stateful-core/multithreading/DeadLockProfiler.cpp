// lib-stateful-core/multithreading/DeadLockProfiler.hpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/multithreading/DeadLockProfiler.hpp>

namespace StatefulCore
{
	namespace Multithreading
	{
		void DeadLockProfiler::PushLock(LockName name)
		{
			LockGrd guard(m_lock);

			LockId id = 0;
			auto findItr = m_nameToId.find(name);

			// Create id or find id.
			if (findItr == m_nameToId.end())
			{
				id = static_cast<int32>(m_nameToId.size());
				m_nameToId[name] = id;
				m_idToName[id] = name;
			}
			else
			{
				id = findItr->second;
			}

			if (l_lockStack.empty() == false)
			{
				const LockId prevId = l_lockStack.top();

				// Connect new lock to previous pushed lock.
				if (prevId != id)
				{
					TreeSet<LockId>& connectedNodes = m_lockGraph[prevId];
					if (connectedNodes.find(id) == connectedNodes.end())
					{
						connectedNodes.insert(id);
						CheckCycle();
					}
				}
			}

			l_lockStack.push(id);
		}

		void DeadLockProfiler::PopLock(LockName name)
		{
			LockGrd guard(m_lock);

			// Check multiple releasing.
			assert(l_lockStack.empty() == false);

			LockId id = m_nameToId[name];

			// Check invalid order.
			assert(l_lockStack.top() == id);

			l_lockStack.pop();
		}

		void DeadLockProfiler::CheckCycle()
		{
			const int32 lockCount = static_cast<int32>(m_nameToId.size());
			m_discoveredHistory = Vec<LockId>(lockCount, -1);
			m_discoveredOrder = 0;
			m_finished = Vec<bool>(lockCount, false);
			m_parent = Vec<LockId>(lockCount, -1);

			for (LockId id = 0; id < lockCount; id++)
				Dfs(id);

			m_discoveredHistory.clear();
			m_finished.clear();
			m_parent.clear();
		}

		void DeadLockProfiler::Dfs(LockId id)
		{
			if (m_discoveredHistory[id] != -1)
				return;

			m_discoveredHistory[id] = m_discoveredOrder++;

			auto findItr = m_lockGraph.find(id);

			if (findItr == m_lockGraph.end())
			{
				m_finished[id] = true;
				return;
			}

			TreeSet<LockId>& nextLocks = findItr->second;

			for (LockId nextId : nextLocks)
			{
				if (m_discoveredHistory[nextId] == -1)
				{
					m_parent[nextId] = id;
					Dfs(nextId);
					continue;
				}

				// Forward edge
				if (m_discoveredHistory[id] < m_discoveredHistory[nextId])
					continue;

				// if (DFS of nextId is in progress) -> (id, nextId) is back edge
				// else -> (id, nextId) is cross edge
				if (m_finished[nextId] == false)
				{
					printf_s("%s -> %s\n", m_idToName[id], m_idToName[nextId]);

					int32 curId = id;
					while (true)
					{
						printf_s("%s -> %s\n", m_idToName[m_parent[curId]], m_idToName[curId]);
						curId = m_parent[curId];

						// Detect deadlock
						assert(curId != nextId);
					}
				}
			}

			m_finished[id] = true;
		}
	}
}

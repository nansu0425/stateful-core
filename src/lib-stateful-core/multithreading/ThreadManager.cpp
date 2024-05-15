// lib-stateful-core/thread/ThreadManager.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>

namespace StatefulCore
{
	namespace Multithreading
	{
		ThreadManager::ThreadManager()
		{
			l_threadId = 1;
		}

		ThreadManager::~ThreadManager()
		{
			Join();
		}

		void ThreadManager::Launch(Func<void()> callback)
		{
			LockGrd guard(m_lock);

			m_threads.push_back(
				Thread([this, callback]()
					{
						this->InitTLS();
						callback();
						this->DestroyTLS();
					}
				)
			);
		}
		
		void ThreadManager::Join()
		{
			for (Thread& t : m_threads)
			{
				if (t.joinable())
					t.join();
			}

			m_threads.clear();
			m_threadIds.clear();
		}

		void ThreadManager::InitTLS()
		{
			l_threadId = GetNextThreadId();
		}

		void ThreadManager::DestroyTLS()
		{

		}

		ThreadId ThreadManager::GetNextThreadId()
		{
			static Atomic<ThreadId> s_nextId = 2;
			ThreadId id = s_nextId.load();

			while (true)
			{
				if (s_nextId.compare_exchange_weak(OUT id, id + 1))
				{
					LockGrd guard(m_lock);

					if (m_threadIds.insert(id).second)
						break;
				}
			}

			return id;
		}
	}
}
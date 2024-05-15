// lib-stateful-core/thread/ThreadManager.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/multithreading/ThreadManager.hpp>

namespace StatefulCore
{
	namespace Multithreading
	{
		ThreadManager::ThreadManager()
		{
			InitTLS();
		}

		ThreadManager::~ThreadManager()
		{
			Join();
		}

		void ThreadManager::Launch(Func<void()> callback)
		{
			LockGrd lockGrd(m_lock);

			m_threads.push_back(
				Thread([=]()
					{
						InitTLS();
						callback();
						DestroyTLS();
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
		}

		void ThreadManager::InitTLS()
		{
			static Atomic<ThreadId> s_nextThreadId = 1;
			l_threadId = s_nextThreadId.fetch_add(1);
		}

		void ThreadManager::DestroyTLS()
		{

		}
	}
}
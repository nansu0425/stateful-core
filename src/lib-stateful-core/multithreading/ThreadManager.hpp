// lib-stateful-core/thread/ThreadManager.hpp

#pragma once

namespace StatefulCore
{
	namespace Multithreading
	{
		/*---------------------*
		 *    ThreadManager    *
		 *---------------------*/
		class ThreadManager
		{
		public:
			ThreadManager();
			~ThreadManager();

		public:
			void Launch(Func<void()> callback);
			void Join();

		private:
			void InitTLS();
			void DestroyTLS();

			ThreadId GetNextThreadId();

		private:
			Mutex              m_lock;
			VecThread          m_threads;
			HashSetThreadId    m_threadIds;
		};
	}
}
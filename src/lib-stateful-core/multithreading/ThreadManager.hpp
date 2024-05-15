// lib-stateful-core/thread/ThreadManager.hpp

#pragma once

namespace StatefulCore
{
	namespace Multithreading
	{
		class ThreadManager
		{
		public:
			ThreadManager();
			~ThreadManager();

		public:
			void Launch(Func<void()> callback);
			void Join();

			static void InitTLS();
			static void DestroyTLS();

		private:
			Mutex        m_lock;
			VecThread    m_threads;
		};
	}
}
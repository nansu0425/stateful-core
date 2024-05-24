// lib-stateful-core/asynchronous/JobQueue.hpp

#pragma once

#include <lib-stateful-core/multithreading/LockQueue.hpp>
#include <lib-stateful-core/asynchronous/Job.hpp>

namespace StatefulCore
{
	namespace Asynchronous
	{
		/*----------------*
		 *    JobQueue    *
		 *----------------*/

		class JobQueue : public std::enable_shared_from_this<JobQueue>
		{
		public:
			void DoAsync(CallbackFunc&& callback)
			{
				Push(Memory::ObjectPool<Job>::MakeShared(std::move(callback)));
			}

			template<typename C, typename Ret, typename... Args>
			void DoAsync(Ret(C::* method)(Args...), Args... args)
			{
				SPtr<C> owner = std::static_pointer_cast<C>(shared_from_this());
				Push(Memory::ObjectPool<Job>::MakeShared(owner, method, std::forward<Args>(args)...));
			}

			void ClearJobs() { m_jobs.Clear(); }

		private:
			void Push(SPtr<Job> job, bool pushOnly = false);
			void Execute();

		protected:
			Multithreading::LockQueue<SPtr<Job>> m_jobs;
			Atomic<int32> m_numJobs = 0;
		};
	}
}

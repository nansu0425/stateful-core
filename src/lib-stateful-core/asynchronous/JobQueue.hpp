// lib-stateful-core/asynchronous/JobQueue.hpp

#pragma once

#include <lib-stateful-core/multithreading/LockQueue.hpp>
#include <lib-stateful-core/asynchronous/Job.hpp>
#include <lib-stateful-core/asynchronous/ReservedJobQueue.hpp>

namespace StatefulCore
{
	namespace Asynchronous
	{
		/*----------------*
		 *    JobQueue    *
		 *----------------*/

		class JobQueue : public std::enable_shared_from_this<JobQueue>
		{
		private:
			friend class ReservedJobQueue;
			friend class JobQueueManager;

		public:
			void DoAsync(CallbackFunc&& callback)
			{
				Push(Memory::ObjectPool<Job>::MakeShared(shared_from_this(), std::move(callback)));
			}

			template<typename C, typename Ret, typename... Args>
			void DoAsync(Ret(C::* method)(Args...), Args... args)
			{
				SPtr<C> self = std::static_pointer_cast<C>(shared_from_this());

				Push(Memory::ObjectPool<Job>::MakeShared(
					shared_from_this(), self, method, std::forward<Args>(args)...));
			}

			void DoAsync(Tick64 waitingTick, CallbackFunc&& callback)
			{
				SPtr<Job> job = Memory::ObjectPool<Job>::MakeShared(
					shared_from_this(), std::move(callback));

				g_reservedJobQueue->Reserve(waitingTick, job);
			}

			template<typename C, typename Ret, typename... Args>
			void DoAsync(Tick64 waitingTick, Ret(C::* method)(Args...), Args... args)
			{
				SPtr<C> self = std::static_pointer_cast<C>(shared_from_this());
				SPtr<Job> job = Memory::ObjectPool<Job>::MakeShared(
					shared_from_this(), self, method, std::forward<Args>(args)...);

				g_reservedJobQueue->Reserve(waitingTick, job);
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

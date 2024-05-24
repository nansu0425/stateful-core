// lib-stateful-core/asynchronous/ReservedJobQueue.hpp

#pragma once

namespace StatefulCore
{
	namespace Asynchronous
	{
		struct ReservedJob
		{
			Tick64       execute = 0;
			SPtr<Job>    job = nullptr;

			bool operator<(const ReservedJob& other) const
			{
				return execute > other.execute;
			}
		};

		/*------------------------*
		 *    ReservedJobQueue    *
		 *------------------------*/

		class ReservedJobQueue
		{
		public:
			void    Reserve(Tick64 wait, SPtr<Job> job);
			void    Distribute2JobQueueManager(Tick64 now);

			static void    Distribute2JobQueueManager();

		private:
			USE_RW_SPIN_LOCK;
			Memory::PriorityQueue<ReservedJob>    m_resvJobs;
			Atomic<bool>    m_distributing = false;
		};
	}
}

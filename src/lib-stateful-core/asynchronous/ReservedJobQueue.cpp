// lib-stateful-core/asynchronous/ReservedJobQueue.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/asynchronous/ReservedJobQueue.hpp>

namespace StatefulCore
{
	namespace Asynchronous
	{
		void ReservedJobQueue::Reserve(Tick64 wait, SPtr<Job> job)
		{
			const Tick64 execute = ::GetTickCount64() + wait;

			W_SPIN_LOCK;
			m_resvJobs.push(ReservedJob{ execute, job });
		}

		void ReservedJobQueue::Distribute2JobQueueManager(Tick64 now)
		{
			// Allow only single thread for maintaining job push order.
			if (m_distributing.exchange(true) == true)
				return;

			Memory::Vector<ReservedJob> resvJobs;

			{
				W_SPIN_LOCK;

				while (m_resvJobs.empty() == false)
				{
					const ReservedJob& resvJob = m_resvJobs.top();
					if (now < resvJob.execute)
						break;

					resvJobs.push_back(resvJob);
					m_resvJobs.pop();
				}
			}

			for (ReservedJob& resvJob : resvJobs)
			{
				if (SPtr<JobQueue> owner = resvJob.job->GetOwner())
					owner->Push(resvJob.job, true);
			}

			m_distributing.store(false);
		}

		void ReservedJobQueue::Distribute2JobQueueManager()
		{
			const Tick64 now = ::GetTickCount64();

			g_reservedJobQueue->Distribute2JobQueueManager(now);
		}
	}
}

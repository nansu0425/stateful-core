// lib-stateful-core/asynchronous/JobQueue.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/asynchronous/JobQueue.hpp>
#include <lib-stateful-core/asynchronous/JobQueueManager.hpp>

namespace StatefulCore
{
	namespace Asynchronous
	{
		void JobQueue::Push(SPtr<Job> job, bool pushOnly)
		{
			const int32 prevNum = m_numJobs.fetch_add(1);
			m_jobs.Push(job);

			// If the job is first item
			if (prevNum == 0)
			{
				if (l_execJobQueue == nullptr && pushOnly == false)
				{
					Execute();
				}
				else
				{
					g_jobQueueManager->Push(shared_from_this());
				}
			}
		}

		void JobQueue::Execute()
		{
			l_execJobQueue = shared_from_this();

			while (true)
			{
				Memory::Vector<SPtr<Job>> jobs;
				m_jobs.PopAll(OUT jobs);

				const int32 numJobs = static_cast<int32>(jobs.size());
				for (int32 i = 0; i < numJobs; i++)
					jobs[i]->Execute();

				// If queue is empty
				if (m_numJobs.fetch_sub(numJobs) == numJobs)
				{
					l_execJobQueue = nullptr;
					break;
				}

				const Tick64 now = ::GetTickCount64();

				// If main cycle end time has been exceeded
				if (now >= l_mainCycleEnd)
				{
					l_execJobQueue = nullptr;
					g_jobQueueManager->Push(shared_from_this());
					break;
				}
			}
		}
	}
}
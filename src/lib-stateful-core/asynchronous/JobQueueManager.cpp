// lib-stateful-core/asynchronous/JobQueueManager.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/asynchronous/JobQueueManager.hpp>

namespace StatefulCore
{
	namespace Asynchronous
	{
		void JobQueueManager::Push(SPtr<JobQueue> jobQueue)
		{
			m_jobQueues.Push(jobQueue);
		}

		SPtr<JobQueue> JobQueueManager::Pop()
		{
			return m_jobQueues.Pop();
		}

		void JobQueueManager::ProcessJobQueues()
		{
			while (true)
			{
				Tick64 now = ::GetTickCount64();
				if (now > l_mainCycleEnd)
					break;

				SPtr<JobQueue> jobQueue = g_jobQueueManager->Pop();
				if (jobQueue == nullptr)
					break;

				jobQueue->Execute();
			}
		}
	}
}

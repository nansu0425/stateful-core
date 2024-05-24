// lib-stateful-core/asynchronous/JobQueueManager.hpp

#pragma once

#include <lib-stateful-core/multithreading/LockQueue.hpp>

namespace StatefulCore
{
	namespace Asynchronous
	{
		/*-----------------------*
		 *    JobQueueManager    *
		 *-----------------------*/

		class JobQueueManager
		{
		public:
			void              Push(SPtr<JobQueue> jobQueue);
			SPtr<JobQueue>    Pop();

		private:
			Multithreading::LockQueue<SPtr<JobQueue>> m_jobQueues;
		};
	}
}

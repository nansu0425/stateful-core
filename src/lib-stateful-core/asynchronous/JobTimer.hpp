// lib-stateful-core/asynchronous/JobTimer.hpp

#pragma once

namespace StatefulCore
{
	namespace Asynchronous
	{
		struct TimerItem
		{
			Tick64       execute = 0;
			SPtr<Job>    job = nullptr;

			bool operator<(const TimerItem& other) const
			{
				return execute > other.execute;
			}
		};

		/*----------------*
		 *    JobTimer    *
		 *----------------*/

		class JobTimer
		{
		public:
			void    Reserve(Tick64 wait, SPtr<Job> job);
			void    Distribute2JobQueues(Tick64 now);

			static void    Distribute2JobQueues();

		private:
			USE_RW_SPIN_LOCK;
			Memory::PriorityQueue<TimerItem>    m_items;
			Atomic<bool>    m_distributing = false;
		};
	}
}

// lib-stateful-core/asynchronous/JobTimer.hpp

#pragma once

namespace StatefulCore
{
	namespace Asynchronous
	{
		struct TimerItem
		{
			uint64       tickExecute = 0;
			SPtr<Job>    job = nullptr;

			bool operator<(const TimerItem& other) const
			{
				return tickExecute > other.tickExecute;
			}
		};

		/*----------------*
		 *    JobTimer    *
		 *----------------*/

		class JobTimer
		{
		public:
			void    Reserve(uint64 waitingTick, SPtr<Job> job);
			void    Distribute(uint64 now);

		private:
			USE_RW_SPIN_LOCK;
			Memory::PriorityQueue<TimerItem>    m_items;
			Atomic<bool>    m_distributing = false;
		};
	}
}

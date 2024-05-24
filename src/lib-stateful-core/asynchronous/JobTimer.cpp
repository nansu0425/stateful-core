// lib-stateful-core/asynchronous/JobTimer.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/asynchronous/JobTimer.hpp>

namespace StatefulCore
{
	namespace Asynchronous
	{
		void JobTimer::Reserve(Tick64 waitingTick, SPtr<Job> job)
		{
			const Tick64 tickExecute = ::GetTickCount64() + waitingTick;

			W_SPIN_LOCK;
			m_items.push(TimerItem{ tickExecute, job });
		}

		void JobTimer::Distribute(Tick64 curTick)
		{
			// Allow only single thread for maintaining job push order.
			if (m_distributing.exchange(true) == true)
				return;

			Memory::Vector<TimerItem> itemsExec;

			{
				W_SPIN_LOCK;

				while (m_items.empty() == false)
				{
					const TimerItem& item = m_items.top();
					if (curTick < item.tickExecute)
						break;

					itemsExec.push_back(item);
					m_items.pop();
				}
			}

			for (TimerItem& item : itemsExec)
			{
				if (SPtr<JobQueue> owner = item.job->GetOwner())
					owner->Push(item.job, true);
			}

			m_distributing.store(false);
		}
	}
}

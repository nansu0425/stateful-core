// lib-stateful-core/asynchronous/JobTimer.cpp

#include <lib-stateful-core/precompiled/Pch.hpp>
#include <lib-stateful-core/asynchronous/JobTimer.hpp>

namespace StatefulCore
{
	namespace Asynchronous
	{
		void JobTimer::Reserve(Tick64 wait, SPtr<Job> job)
		{
			const Tick64 execute = ::GetTickCount64() + wait;

			W_SPIN_LOCK;
			m_items.push(TimerItem{ execute, job });
		}

		void JobTimer::Distribute2JobQueues(Tick64 now)
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
					if (now < item.execute)
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

		void JobTimer::Distribute2JobQueues()
		{
			const Tick64 now = ::GetTickCount64();

			g_jobTimer->Distribute2JobQueues(now);
		}
	}
}

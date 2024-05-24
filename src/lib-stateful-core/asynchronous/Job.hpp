// lib-stateful-core/asynchronous/Job.hpp

#pragma once

namespace StatefulCore
{
	namespace Asynchronous
	{
		using CallbackFunc = std::function<void()>;

		/*-----------*
		 *    Job    *
		 *-----------*/

		class Job
		{
		public:
			Job(SPtr<JobQueue> owner, CallbackFunc&& callback)
				: m_owner(owner), m_callback(std::move(callback)) { }

			template<typename C, typename Ret, typename... Args>
			Job(SPtr<JobQueue> owner, SPtr<C> obj, Ret(C::* method)(Args...), Args&&... args)
				: m_owner(owner)
			{
				m_callback = [obj, method, args...]()
					{
						(obj.get()->*method)(args...);
					};
			}

		public:
			void Execute()
			{
				assert(m_owner.expired() == false);
				m_callback();
				m_owner.reset();
			}

			SPtr<JobQueue> GetOwner() { return m_owner.lock(); }

		private:
			WPtr<JobQueue> m_owner;
			CallbackFunc m_callback;
		};
	}
}

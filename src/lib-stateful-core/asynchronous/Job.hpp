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
			Job(CallbackFunc&& callback) : m_callback(std::move(callback)) { }

			template<typename C, typename Ret, typename... Args>
			Job(SPtr<C> owner, Ret(C::* method)(Args...), Args&&... args)
			{
				m_callback = [owner, method, args...]()
					{
						(owner.get()->*method)(args...);
					};
			}

			void Execute()
			{
				m_callback();
			}

		private:
			CallbackFunc m_callback;
		};
	}
}

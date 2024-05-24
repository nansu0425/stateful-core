// lib-stateful-core/multithreading/LockQueue.hpp

#pragma once

namespace StatefulCore
{
	namespace Multithreading
	{
		/*-----------------*
		 *    LockQueue    *
		 *-----------------*/

		template<typename T>
		class LockQueue
		{
		public:
			void Push(T item)
			{
				W_SPIN_LOCK;

				m_items.push(item);
			}

			T Pop()
			{
				W_SPIN_LOCK;

				if (m_items.empty())
					return T();

				T item = m_items.front();
				m_items.pop();

				return item;
			}

			void PopAll(OUT Memory::Vector<T>& items)
			{
				W_SPIN_LOCK;

				while (T item = Pop())
					items.push_back(item);
			}

			void Clear()
			{
				W_SPIN_LOCK;

				m_items = Memory::Queue<T>();
			}

		private:
			USE_RW_SPIN_LOCK;
			Memory::Queue<T> m_items;
		};
	}
}

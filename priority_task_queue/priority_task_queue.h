#ifndef INCLUDED_PRIORITYTASKQUEUE_
#define INCLUDED_PRIORITYTASKQUEUE_

#include <queue>
#include <utility>

namespace wp
{
	template <typename Functor, typename ...Args>
	class priority_task_queue
	{

		private:
			std::priority_queue<Functor, Args...> d_prio;

		public:

			template <typename ...Params>
			priority_task_queue(Params&&... params)
			:
				d_prio(std::forward<Params>...)
			{}

			size_t size() const
			{
				return d_prio.size();
			}

			bool empty() const
			{
				return d_prio.empty();
			}

			// returns a non-const reference to the first element in the underlying
			// priority_queue, only use for moving the element before pop-ing it
			Functor &front()
			{
				return const_cast<Functor&>(d_prio.top());
			}

			void pop()
			{
				d_prio.pop();
			}

			void push(Functor &&value)
			{
				d_prio.push(std::move(value));
			}

	};
}

#endif
#ifndef INCLUDED_PRIORITYTASK_
#define INCLUDED_PRIORITYTASK_

#include <cstddef>

namespace wp
{
	template <typename Functor> // Functor should be of type void(void)
	class priority_task
	{

		private:
			Functor d_functor;
			size_t d_priority;

		public:

			template <typename T>
			friend bool operator<(priority_task<T> const &task1,
				                  priority_task<T> const &task2);

			priority_task(Functor &&func, size_t priority = 0);

			// call the functor
			void operator()()
			{
				d_functor();
			}

			void priority(size_t prio)
			{
				d_priority = prio;
			}

			size_t priority() const
			{
				return d_priority;
			}

	};
}

#include "priority_task.inl"
#include "operator_less.inl"

#endif
#ifndef INCLUDED_worker_pool_
#define INCLUDED_worker_pool_

#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <list>
#include <iterator>
#include <functional>

namespace wp
{
	template <typename Functor, class Container = std::queue<Functor, std::list<Functor> >,
			  Functor &(Container::*Next)(void) = &Container::front,
			  void(Container::*Pop)(void) = &Container::pop,
			  void(Container::*Push)(Functor&&) = &Container::push >
	class worker_pool
	{
		mutable std::mutex      d_workMut;        // synchronizes d_tasks, d_workers
		                                          // d_paused and d_waiting
		mutable std::mutex      d_countMut;       // sinchronizes calls to
		                                          // void workerCount(size_t)
		std::condition_variable d_taskCond;       // notifies when tasks are available
		                                          // or when work is resumed
		std::condition_variable d_doneCond;       // notifies when work is finished
		std::list<std::thread>  d_workers;        // list of worker threads
		Container               d_tasks;          // queue of tasks
		size_t                  d_waiting = 0;    // number of inactive workers
		bool                    d_paused = false; // while true, workers can't pop tasks

		public:
			worker_pool(size_t nWorkers, Container &&container = Container());

			// noncopyable, nonmovable
			worker_pool(worker_pool const &other) = delete;
			worker_pool(worker_pool &&tmp) = delete;

			~worker_pool();

			// noncopyable, nonmovable
			worker_pool &operator=(worker_pool const &other) = delete;
			worker_pool &operator=(worker_pool &&tmp) = delete;

			void push(Functor &&task);              // add task to queue

			template <typename InputIt>             // add tasks from range
			void push(InputIt first, InputIt last); // [first, last) to queue

			void worker_count(size_t nWorkers);     // change number of worker threads
			void pause();                           // stop workers from poping tasks
			void resume();                          // allow threads to resume work
			void wait_finish();                     // wait until there are no tasks
			                                        // left in the queue and all the
													// worker threads are done

			bool is_paused() const;                 // returns d_paused's value

			size_t worker_count() const;            // return number of worker threads
			size_t task_count() const;              // return number of tasks in queue

		private:
			void work(size_t id);                   // used for threads
	};
}

#include "worker_pool.inl"
#include "destructor.inl"
#include "is_paused.inl"
#include "pause.inl"
#include "push1.inl"
#include "push2.inl"
#include "resume.inl"
#include "task_count.inl"
#include "wait_finish.inl"
#include "worker_count1.inl"
#include "worker_count2.inl"
#include "work.inl"

#endif
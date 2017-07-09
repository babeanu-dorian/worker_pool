// get task from queue, if there are none or the worker pool is paused, wait
namespace wp
{
	template <typename Functor, class Container, Functor &(Container::*Next)(void),
	          void(Container::*Pop)(void), void(Container::*Push)(Functor&&)>
	void worker_pool<Functor, Container, Next, Pop, Push>::work(size_t id)
	{
		while (true)
		{
			std::unique_lock<std::mutex> taskLock(d_workMut);

			// check if thread is supposed to stop
			if (id >= d_workers.size())
				return;

			// check if there are tasks in the queue or if the pool is paused
			while (d_tasks.empty() || d_paused)
			{
				// notify that the work is possibly done
				if (d_tasks.empty() && (d_waiting + 1 == d_workers.size()))
					d_doneCond.notify_all();

				// wait for tasks to be pushed
				++d_waiting;
				d_taskCond.wait(taskLock);
				--d_waiting;

				// check if thread is supposed to stop
				if (id >= d_workers.size())
					return;
			}

			// dequeue task
			Functor task = std::move((d_tasks.*Next)());
			(d_tasks.*Pop)();

			taskLock.unlock();

			task(); // execute task
		}
	}
}
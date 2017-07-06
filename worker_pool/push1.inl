namespace wp
{
	template <typename Functor, class Container, Functor &(Container::*Next)(void),
	          void(Container::*Pop)(void), void(Container::*Push)(Functor&&)>
	void worker_pool<Functor, Container, Next, Pop, Push>::push(Functor &&task)
	{
		{
			std::lock_guard<std::mutex> lg(d_workMut);
			(d_tasks.*Push)(std::move(task));
		}
		d_taskCond.notify_one();
	}
}
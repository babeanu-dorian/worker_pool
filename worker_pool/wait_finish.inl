namespace wp
{
	template <typename Functor, class Container, Functor &(Container::*Next)(void),
	          void(Container::*Pop)(void), void(Container::*Push)(Functor&&)>
	void worker_pool<Functor, Container, Next, Pop, Push>::wait_finish()
	{
		std::unique_lock<std::mutex> lock(d_workMut);
		while (!d_tasks.empty() || (d_waiting < d_workers.size()))
			d_doneCond.wait(lock);
	}
}
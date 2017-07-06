namespace wp
{
	template <typename Functor, class Container, Functor &(Container::*Next)(void),
	          void(Container::*Pop)(void), void(Container::*Push)(Functor&&)>
	void worker_pool<Functor, Container, Next, Pop, Push>::resume()
	{
		std::lock_guard<std::mutex> lg(d_workMut);
		d_paused = false;
		if (!d_tasks.empty())
			d_taskCond.notify_all();
	}
}
namespace wp
{
	template <typename Functor, class Container, Functor &(Container::*Next)(void),
	          void(Container::*Pop)(void), void(Container::*Push)(Functor&&)>
	template <typename InputIt> 
	void worker_pool<Functor, Container, Next, Pop, Push>::push(InputIt first, InputIt last)
	{
		{
			std::lock_guard<std::mutex> lg(d_workMut);
			for (; first != last; ++first)
				(d_tasks.*Push)(std::move(*first));
		}
		d_taskCond.notify_all();
	}
}
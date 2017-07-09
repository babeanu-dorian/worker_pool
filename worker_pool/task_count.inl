namespace wp
{
	template <typename Functor, class Container, Functor &(Container::*Next)(void),
	          void(Container::*Pop)(void), void(Container::*Push)(Functor&&)>
	size_t worker_pool<Functor, Container, Next, Pop, Push>::task_count() const
	{
		std::lock_guard<std::mutex> lg(d_workMut);
		return d_tasks.size();
	}
}
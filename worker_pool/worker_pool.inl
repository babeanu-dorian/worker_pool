namespace wp
{
	template <typename Functor, class Container, Functor &(Container::*Next)(void),
	          void(Container::*Pop)(void), void(Container::*Push)(Functor&&) >
	worker_pool<Functor, Container, Next, Pop, Push>::worker_pool(size_t nWorkers, Container &&container)
	:
		d_tasks(std::move(container))
	{
		worker_count(nWorkers);
	}
}
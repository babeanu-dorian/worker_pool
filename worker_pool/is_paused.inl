namespace wp
{
	template <typename Functor, class Container, Functor &(Container::*Next)(void),
	          void(Container::*Pop)(void), void(Container::*Push)(Functor&&)>
	bool worker_pool<Functor, Container, Next, Pop, Push>::is_paused() const
	{
		std::lock_guard<std::mutex> lg(d_workMut);
		return d_paused;
	}
}
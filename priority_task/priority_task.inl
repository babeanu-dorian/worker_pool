namespace wp
{
	template <typename Functor>
	priority_task<Functor>::priority_task(Functor &&func, size_t priority)
	:
		d_functor(std::move(func)),
		d_priority(priority)
	{}
}
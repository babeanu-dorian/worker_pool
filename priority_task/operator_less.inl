namespace wp
{
	template <typename Functor>
	bool operator<(priority_task<Functor> const &task1, priority_task<Functor> const &task2)
	{
		return task1.d_priority < task2.d_priority;
	}
}
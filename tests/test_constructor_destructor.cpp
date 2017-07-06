#include "tests.h"

// tasks are function pointers, the container is the default one
TEST(ConstructorDestructor, FunctionPointerDefaultContainer)
{
    wp::worker_pool<void(*)(void)> wPool(4);
    ASSERT_EQ(wPool.worker_count(), static_cast<size_t>(4));
}

// tasks are wp::priority_task< std::function > objects,
// the container is wp::priority_task_queue
TEST(ConstructorDestructor, PriorityTaskFunctionObjectPriorityQueue)
{
	typedef wp::priority_task< std::function<void(void)> > Task;
	wp::worker_pool<Task, wp::priority_task_queue<Task> > wPool(8);
    ASSERT_EQ(wPool.worker_count(), static_cast<size_t>(8));
}

// tasks are std::function objects, the container is a queue on a deque
TEST(ConstructorDestructor, FunctionObjectQueueOnDeque)
{
	wp::worker_pool<std::function<void(void)>, std::queue< std::function<void(void)>,
	               std::deque< std::function<void(void)> > > > wPool(5);
	ASSERT_EQ(wPool.worker_count(), static_cast<size_t>(5));
}

// tasks are std::packaged_tasks, the container is the default one
TEST(ConstructorDestructor, PackagedTaskDefaultContainer)
{
	wp::worker_pool<std::packaged_task<int(void)> > wPool(3);
	ASSERT_EQ(wPool.worker_count(), static_cast<size_t>(3));
}

// tasks are std::packaged_tasks, the container is the default one,
// tasks are first pushed in the container, then the container
// is used to construct the wp::worker_pool
TEST(ConstructorDestructor, PackagedTaskDefaultContainerPassedIn)
{
	std::queue<std::packaged_task<size_t(void)>,
	           std::list< std::packaged_task<size_t(void)> > > taskQueue;
	std::vector< std::future<size_t> > results;
	results.reserve(100);

	for (size_t i = 0; i != 100; ++i)
	{
		std::packaged_task<size_t(void)> task(std::bind(simulateWork, i));
		results.push_back(task.get_future());
		taskQueue.push(std::move(task));
	}

	wp::worker_pool<std::packaged_task<size_t(void)> > wPool(4, std::move(taskQueue));
	ASSERT_EQ(wPool.worker_count(), static_cast<size_t>(4));
	wPool.wait_finish();

	// check results
	for (size_t i = 0; i != 100; ++i)
		ASSERT_EQ(results[i].get(), simulateWork(i));
}

// tasks are function objects produced by std::bind,
// the container is a deque
TEST(ConstructorDestructor, FunctionBindObjectDeque)
{
	using bound_t = decltype(std::bind(simulateWork, 0));
	wp::worker_pool<bound_t, std::deque<bound_t>,
	               &std::deque<bound_t>::front,
	               &std::deque<bound_t>::pop_front,
	               &std::deque<bound_t>::push_back > wPool(6);
	ASSERT_EQ(wPool.worker_count(), static_cast<size_t>(6));
}
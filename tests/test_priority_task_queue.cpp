#include "tests.h"

void checkDescendingOrder(int cur, int &prev, std::mutex &mut)
{
	std::lock_guard<std::mutex> lock(mut);
	ASSERT_LE(cur, prev);
	prev = cur;
}

// check ordering of tasks
TEST(PriorityTaskQueue, Order)
{
	typedef wp::priority_task< std::function<void(void)> > PrioTask;
	// use one worker thread to check the ordering of tasks
	wp::worker_pool<PrioTask, wp::priority_task_queue<PrioTask> > wPool(1);
	wPool.pause();

	std::srand(std::time(nullptr));
	int prev = RAND_MAX;
	std::mutex mut;
	for (size_t i = 100; i > 0; --i)
	{
		int cur = std::rand();
		wPool.push(PrioTask(
			std::bind(checkDescendingOrder, cur, std::ref(prev), std::ref(mut)),
			static_cast<size_t>(cur)));
	}

	wPool.resume();
	wPool.wait_finish();

	// check if all tasks have been performed
	ASSERT_EQ(wPool.task_count(), static_cast<size_t>(0));
}

// check integrity of results
TEST(PriorityTaskQueue, Results)
{
	typedef wp::priority_task< std::packaged_task<size_t(void)> > PrioTask;
	wp::worker_pool<PrioTask, wp::priority_task_queue<PrioTask> > wPool(4);
	std::vector< std::future<size_t> > results;
	results.reserve(200);

	for (size_t i = 1; i < 201; ++i)
	{
		std::packaged_task<size_t(void)> task(std::bind(simulateWork, i));
		results.push_back(task.get_future());
		wPool.push(PrioTask(std::move(task), i)); // priority based on workload
	}

	wPool.wait_finish();
	ASSERT_EQ(wPool.task_count(), static_cast<size_t>(0));

	// check results
	for (size_t i = 0; i < 200; ++i)
		ASSERT_EQ(results[i].get(), simulateWork(i+1));
}
#include "tests.h"

TEST(WorkerCount, ChangeNumberOfWorkerThreadsWhileExecutingTasks)
{
	wp::worker_pool< std::packaged_task<size_t(void)> > wPool(4);
	wPool.pause();
	std::vector< std::future<size_t> > results;
	results.reserve(100);

	for (size_t i = 100; i != 0; --i)
	{
		std::packaged_task<size_t(void)> task{std::bind(simulateWork, i)};
		results.push_back(task.get_future());
		wPool.push(std::move(task));
	}

	wPool.resume();

	// alternate between increasing and decreasing the number of workers
	// while tasks are being performed
	for (size_t i = 1; i < 5; ++i)
	{
		wPool.worker_count(4 - i);
		ASSERT_EQ(wPool.worker_count(), 4 - i);

		wPool.worker_count(4 + i);
		ASSERT_EQ(wPool.worker_count(), 4 + i);
	}

	wPool.wait_finish();

	// check results
	for (size_t i = 0; i != 100; ++i)
		ASSERT_EQ(results[i].get(), simulateWork(100 - i));
}
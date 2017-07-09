#include "tests.h"

TEST(PauseResume, PauseToPushTasks)
{
	size_t count = 0;
	std::mutex mut;
	wp::worker_pool< std::function<void(void)> > wPool(4);
	wPool.pause();
	ASSERT_TRUE(wPool.is_paused());
	for (size_t i = 0; i < 100; ++i)
		wPool.push(std::bind(incrementParallelCount, std::ref(mut), std::ref(count)));
	ASSERT_EQ(wPool.task_count(), static_cast<size_t>(100));

	wPool.resume();
	wPool.wait_finish();

	// check if all tasks have been performed
	ASSERT_EQ(wPool.task_count(), static_cast<size_t>(0));
	ASSERT_EQ(count, static_cast<size_t>(100));
}

TEST(PauseResume, PauseWhileExecutingTasks)
{
	size_t count = 0;
	std::mutex mut;
	std::condition_variable pauseCond;
	wp::worker_pool< std::function<void(void)> > wPool(4);
	for (size_t i = 0; i < 100; ++i)
	{
		wPool.push([&]{
			std::lock_guard<std::mutex> lock(mut);
			++count;
			if (count == 50)
			{
				wPool.pause();
				pauseCond.notify_one();
			}
		});
	}

	// wait for pool to be paused
	{
		std::unique_lock<std::mutex> lock(mut);
		while (!wPool.is_paused())
			pauseCond.wait(lock);
	}

	ASSERT_TRUE(wPool.is_paused());

	// number of tasks in queue can be between 47 and 50, depending
	// on what the other threads were doing when pause() was called
	ASSERT_LE(wPool.task_count(), 50);
	ASSERT_GE(wPool.task_count(), 47);

	wPool.resume();
	ASSERT_FALSE(wPool.is_paused());

	wPool.wait_finish();
	// check if all tasks have been performed
	ASSERT_EQ(wPool.task_count(), static_cast<size_t>(0));
	ASSERT_EQ(count, static_cast<size_t>(100));
}

TEST(PauseResume, RedundantCalls)
{
	wp::worker_pool< std::function<void(void)> > wPool(4);

	wPool.pause(); // pauses the pool
	ASSERT_TRUE(wPool.is_paused());

	wPool.pause(); // redundant
	ASSERT_TRUE(wPool.is_paused());

	wPool.pause(); // redundant
	ASSERT_TRUE(wPool.is_paused());

	wPool.resume(); // unpauses the pool
	ASSERT_FALSE(wPool.is_paused());

	wPool.resume(); // redundant
	ASSERT_FALSE(wPool.is_paused());

	wPool.resume(); // redundant
	ASSERT_FALSE(wPool.is_paused());

}
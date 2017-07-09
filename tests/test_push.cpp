#include "tests.h"

// tasks are dequeued while they are enqueued
TEST(Push, OneTaskAtATime)
{
	size_t count = 0;
	std::mutex mut;
	wp::worker_pool< std::function<void(void)> > wPool(4);
	for (size_t i = 0; i < 100; ++i)
		wPool.push(std::bind(incrementParallelCount, std::ref(mut), std::ref(count)));
	wPool.wait_finish();
	// check if all tasks have been performed
	ASSERT_EQ(wPool.task_count(), static_cast<size_t>(0));
	ASSERT_EQ(count, static_cast<size_t>(100));
}

// all tasks are enqueued before the threads start working through them
TEST(Push, RangeOfTasks)
{
	size_t count = 0;
	std::mutex mut;
	wp::worker_pool< std::function<void(void)> > wPool(4);
	std::vector< std::function<void(void)> > tasks(200, std::bind(incrementParallelCount, std::ref(mut), std::ref(count)));
	wPool.push(tasks.begin(), tasks.end());
	wPool.wait_finish();
	// check if all tasks have been performed
	ASSERT_EQ(wPool.task_count(), static_cast<size_t>(0));
	ASSERT_EQ(count, static_cast<size_t>(200));
}
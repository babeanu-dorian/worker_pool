#ifndef INCLUDED_WP_TESTS_
#define INCLUDED_WP_TESTS_

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <future>
#include <exception>
#include <deque>
#include <vector>
#include <queue>
#include <functional>
#include <utility>

#include "gtest/gtest.h"

#include "worker_pool.h"
#include "priority_task.h"
#include "priority_task_queue.h"

size_t simulateWork(size_t n);
void incrementParallelCount(std::mutex &mut, size_t &count);

#endif
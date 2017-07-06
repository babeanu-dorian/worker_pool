#include "tests.h"

void incrementParallelCount(std::mutex &mut, size_t &count)
{
	std::lock_guard<std::mutex> lock(mut);
	++count;
}
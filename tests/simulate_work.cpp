#include "tests.h"

size_t simulateWork(size_t n)
{
	size_t sum = 0;
	for (size_t i = 0; i != n; ++i)
		sum += i*i;
	return sum;
}
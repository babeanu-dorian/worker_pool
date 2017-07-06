#include "tests.h"

bool checkOrder(std::vector<int> const &arr)
{
	for (size_t i = 1; i < arr.size(); ++i)
	{
		if (arr[i] < arr[i - 1])
			return false;
	}

	return true;
}

std::vector<int> genRandVec(size_t size)
{
	std::srand(std::time(nullptr));
	std::vector<int> arr(size);

	for (auto it = arr.begin(); it != arr.end(); ++it)
		*it = std::rand();

	return arr;
}

size_t partition(std::vector<int> &arr, size_t left, size_t right)
{
	size_t pivot = arr[right - 1];
	size_t i = left - 1;

	for (size_t j = left; j != right; ++j)
	{
		if (arr[j] <= pivot)
		{
			++i;
			if (i != j)
				std::swap(arr[i], arr[j]);
		}
	}

	return i;
}

void qSortParallel(wp::worker_pool< std::function<void(void)> > &wPool,
	               std::vector<int> &arr, size_t left, size_t right)
{
	if (left < right)
	{
		size_t p = partition(arr, left, right);
		wPool.push(std::bind(qSortParallel, std::ref(wPool), std::ref(arr), left, p));
		wPool.push(std::bind(qSortParallel, std::ref(wPool), std::ref(arr), p + 1, right));
	}
}

TEST(QickSort, AscendingOrder)
{
	wp::worker_pool< std::function<void(void)> > wPool(4);
	std::vector<int> arr(genRandVec(100000));

	wPool.push(std::bind(qSortParallel, std::ref(wPool), std::ref(arr), 0, arr.size()));
	wPool.wait_finish();

	ASSERT_TRUE(checkOrder(arr));
}
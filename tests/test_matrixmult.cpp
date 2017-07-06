#include "tests.h"

typedef std::vector< std::vector<int> > Matrix;

Matrix generateRandMatrix(size_t width, size_t height)
{
	std::srand(std::time(nullptr));
	Matrix matrix(height, std::vector<int>(width));

	for (auto yIt = matrix.begin(); yIt != matrix.end(); ++yIt)
		for (auto xIt = yIt->begin(); xIt != yIt->end(); ++xIt)
			*xIt = std::rand();

	return matrix;
}

int rowXcol(size_t row, size_t col, Matrix const &a, Matrix const &b)
{
	int sum = 0;
	for (size_t i = 0; i != b.size(); ++i)
		sum += a[row][i] * b[i][col];
	return sum;
}

void matrixMult(Matrix const &a, Matrix const &b)
{
	size_t width = b.front().size();
	std::vector< std::future<int> > result(a.size() * width);
	wp::worker_pool< std::packaged_task<int(void)> > wPool(4);

	for (size_t i = 0; i != result.size(); ++i)
	{
		std::packaged_task<int(void)> task{std::bind(rowXcol, i / width, i % width, std::cref(a), std::cref(b))};
		result[i] = task.get_future();
		wPool.push(std::move(task));
	}

	wPool.wait_finish();

	// check results
	for (size_t i = 0; i != result.size(); ++i)
		ASSERT_EQ(result[i].get(), rowXcol(i / width, i % width, a, b));
}

TEST(MatrixMult, Small)
{
	matrixMult(generateRandMatrix(5, 4), generateRandMatrix(10, 5));
}

TEST(MatrixMult, Big)
{
	matrixMult(generateRandMatrix(100, 100), generateRandMatrix(101, 100));
}
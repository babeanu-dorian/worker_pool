#include "tests.h"

TEST(Fail, BecauseOfMemoryLeak)
{
	int *a = new int;
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
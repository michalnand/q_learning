#include "common.h"
#include "nn_test.h"
#include "nn_test_dynamics.h"

int main()
{
	srand(time(NULL));

/*
	CNNTest *nn_test;

	nn_test = new CNNTest();
	nn_test->process();

	delete nn_test;
*/


	CDynamicsTest *nn_test;

	nn_test = new CDynamicsTest();
	nn_test->process_single_test();

	delete nn_test;

	printf("program done\n");
	return 0;
}

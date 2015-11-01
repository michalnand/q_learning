#include "common.h"
#include "nn_test.h"


int main()
{
	srand(time(NULL));

	CNNTest *nn_test;

	nn_test = new CNNTest();
	nn_test->process();

	delete nn_test;

	printf("program done\n");
	return 0;
}

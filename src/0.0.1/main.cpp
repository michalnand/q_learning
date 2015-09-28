#include "common.h"
#include "robot_brain/q_learning_test.h"

void test()
{
	class CQlearningTest *q_learning_test;
	q_learning_test = new CQlearningTest();

	 //q_learning_test->run(1000);
	//q_learning_test->run(10000000);
	q_learning_test->run(1000000);

	delete q_learning_test;
}

int main()
{
	srand(time(NULL));

	test();

	printf("program done\n");
	return 0;
}

#include "common.h"
#include "controll_test/controll_test.h"

void test()
{
	class CControllTest *controll_test;
	controll_test = new CControllTest();

	controll_test->run(-1);
	delete controll_test;
}

int main()
{
	srand(time(NULL));

	test();

	printf("program done\n");
	return 0;
}

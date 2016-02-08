#include "q_learning_test.h"

CQlearningTest::CQlearningTest()
{
	//environment = new CEnvironment(16); //16 robots
	environment = new CEnvironment(1); //1 robots
}

CQlearningTest::~CQlearningTest()
{
	if (environment != NULL)
	{
		delete environment;
		environment = NULL;
	}
}

void CQlearningTest::run(u32 iterations)
{
	u32 n;
	for (n = 0; n < iterations; n++)
	{
		environment->process();
	}

	std::vector<float> subspace;

	environment->print(subspace);

	printf("test done\n");
}

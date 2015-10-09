#include "q_learning_test.h"

CQlearningTest::CQlearningTest()
{
	//environment = new CEnvironment(16); //16 robots

	environment = new CEnvironment(1); //1 robots

	class CAgent *agent;

	agent = environment->get_agent(0);

	environment_visualisation = new CEnvironment(100, agent, true);
}

CQlearningTest::~CQlearningTest()
{
	if (environment != NULL)
	{
		delete environment;
		environment = NULL;
	}

	if (environment_visualisation != NULL)
	{
		delete environment_visualisation;
		environment_visualisation = NULL;
	}
}

void CQlearningTest::visualisation_main()
{
	while (1)
	{
		environment_visualisation->process();
		usleep(1000*10);
	}
}

void visualisation_main_tmp(CQlearningTest *q_learning_test)
{
	q_learning_test->visualisation_main();
}

void CQlearningTest::run(u32 iterations)
{
	std::thread visualisation_thread(visualisation_main_tmp, this);

	u32 n;
	for (n = 0; n < iterations; n++)
	{
		environment->process();
	}

	std::vector<float> subspace;

	environment->print(subspace);

	visualisation_thread.join();

	printf("test done\n");
}

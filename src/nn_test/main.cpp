#include "common.h"
#include "nn_test.h"
#include "nn_test_dynamics.h"
#include "water.h"

int main()
{
	srand(time(NULL));

/*
	CNNTest *nn_test;

	nn_test = new CNNTest();
	nn_test->process();

	delete nn_test;
*/

/*
	CDynamicsTest *nn_test;

	nn_test = new CDynamicsTest();
	nn_test->process_single_test();

	delete nn_test;
*/

	CWater *water;

	water = new CWater();

	water->log_input_data((char*)"water_results/input_data.txt");

	water->learn(1000);
	water->process((char*)"water_results/clusterization_result.txt");

	water->save_clusters((char*)"water_results/clusters_01.txt");

	delete water;

	printf("program done\n");
	return 0;
}

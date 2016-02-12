#include "common.h"
#include "robot_brain/q_learning_test.h"

/*

void array_test()
{
	u32 i, j, k;
	std::vector<u32> init_vector, idx_vector;

	std::vector<float> idx_vector_f;

	init_vector.push_back(3);
	init_vector.push_back(7);
	init_vector.push_back(12);


	idx_vector.push_back(0);
	idx_vector.push_back(0);
	idx_vector.push_back(0);

	idx_vector_f.push_back(0);
	idx_vector_f.push_back(0);
	idx_vector_f.push_back(0);


	class CArray *arr = new CArray(init_vector);

	for (k = 0; k < init_vector[0]; k++)
	for (j = 0; j < init_vector[1]; j++)
	for (i = 0; i < init_vector[2]; i++)
	{
		float value = i + j*100 + k*1000; //init_vector[1]; // + k*init_vector[1]*init_vector[2];
		idx_vector[0] = k;
		idx_vector[1] = j;
		idx_vector[2] = i;

		idx_vector_f[0] = (1.0*k)/init_vector[0];
		idx_vector_f[1] = (1.0*j)/init_vector[1];
		idx_vector_f[2] = (1.0*i)/init_vector[2];
		arr->set_f(idx_vector_f, value);
	}

	for (k = 0; k < init_vector[0]; k++)
	{
		for (j = 0; j < init_vector[1]; j++)
		{
			for (i = 0; i < init_vector[2]; i++)
			{
				idx_vector[0] = k;
				idx_vector[1] = j;
				idx_vector[2] = i;
				float value = arr->get(idx_vector);

				printf("%6.3f ", value);
			}

			printf("\n");
		}

		printf("\n\n");
	}

	delete arr;
}

*/

void test()
{
	class CQlearningTest *q_learning_test;
	q_learning_test = new CQlearningTest();

	//q_learning_test->run(1000);
	//q_learning_test->run(1000000);
	q_learning_test->run(-1);
	delete q_learning_test;
}

int main()
{
	srand(time(NULL));

	test();

	printf("program done\n");
	return 0;
}

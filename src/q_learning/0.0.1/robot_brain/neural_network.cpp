#include "neural_network.h"


void NeuralNetworkInitStructure_init(
										struct sNeuralNetworkInitStructure *nn_init_structure,
										u32 init_vector_size,
										float weight_range,
										u32 order,
										u32 neuron_type,
										float learning_constant,
										float weight_range_init
									)
{
	u32 i;
	nn_init_structure->init_vector_size = init_vector_size;
	nn_init_structure->init_vector = (u32*)malloc(init_vector_size*sizeof(u32));

	nn_init_structure->weight_range = weight_range;
	nn_init_structure->order = order;
	nn_init_structure->neuron_type = neuron_type;

	nn_init_structure->learning_constant = learning_constant;

	nn_init_structure->weight_range_init = weight_range_init;

	for (i = 0; i < init_vector_size; i++)
		nn_init_structure->init_vector[i] = 0;
}

void NeuralNetworkInitStructure_uninit(struct sNeuralNetworkInitStructure *nn_init_structure)
{
	if (nn_init_structure->init_vector != NULL)
	{
		free(nn_init_structure->init_vector);
		nn_init_structure->init_vector = NULL;
		nn_init_structure->init_vector_size = 0;
	}
}


CNeuralNetwork::CNeuralNetwork(struct sNeuralNetworkInitStructure nn_init_structure)
{
	u32 i, j, k;

	nn.layers_count = nn_init_structure.init_vector_size-1;
	nn.weight_range = nn_init_structure.weight_range;
	nn.learning_constant = nn_init_structure.learning_constant;
	nn.order = nn_init_structure.order;
	nn.neuron_type = nn_init_structure.neuron_type;

	nn.size_input = (u32*)malloc(nn.layers_count*sizeof(u32));
	nn.size_input_ = (u32*)malloc(nn.layers_count*sizeof(u32));
	nn.size_output = (u32*)malloc(nn.layers_count*sizeof(u32));

	for (k = 0; k < nn.layers_count; k++)
	{
		nn.size_input[k] = nn_init_structure.init_vector[k];
		nn.size_output[k] = nn_init_structure.init_vector[k+1];

		u32 _j, _i;
		u32 tmp = 0;

		for (_j = 0; _j < nn.size_input[k]; _j++)
			tmp++;

		for (_j = 0; _j < nn.size_input[k]; _j++)
			for (_i = _j; _i < nn.size_input[k]; _i++)
				tmp++;

		for (_j = 3; _j < nn.order; _j++)
			for (_i = 0; _i < nn.size_input[k]; _i++)
				tmp++;

		nn.size_input_[k] = tmp;
	}

	nn.w = (float***)malloc(nn.layers_count*sizeof(float**));
	nn.dw = (float***)malloc(nn.layers_count*sizeof(float**));

	nn.output = (float**)malloc(nn.layers_count*sizeof(float*));
	nn.error = (float**)malloc(nn.layers_count*sizeof(float*));

	nn.input = (float**)malloc(nn.layers_count*sizeof(float*));
	nn.input_ = (float**)malloc(nn.layers_count*sizeof(float*));

	for (k = 0; k < nn.layers_count; k++)
	{
		nn.w[k] = (float**)malloc(nn.size_output[k]*sizeof(float*));
		nn.dw[k] = (float**)malloc(nn.size_output[k]*sizeof(float*));

		nn.output[k] = (float*)malloc(nn.size_output[k]*sizeof(float));
		nn.error[k] = (float*)malloc(nn.size_output[k]*sizeof(float));
		nn.input[k] = (float*)malloc(nn.size_input[k]*sizeof(float));
		nn.input_[k] = (float*)malloc(nn.size_input_[k]*sizeof(float));

		for (j = 0; j < nn.size_output[k]; j++)
		{

			nn.w[k][j] = (float*)malloc(nn.size_input_[k]*sizeof(float));
			nn.dw[k][j] = (float*)malloc(nn.size_input_[k]*sizeof(float));

			for (i = 0; i < nn.size_input_[k]; i++)
			{
				nn.w[k][j][i] = nn.weight_range*rnd()*nn_init_structure.weight_range_init;
				nn.dw[k][j][i] = 0.0;
			}

			nn.output[k][j] = 0.0;
			nn.error[k][j] = 0.0;
		}

		for (j = 0; j < nn.size_input[k]; j++)
			nn.input[k][j] = 0.0;

		for (j = 0; j < nn.size_input_[k]; j++)
			nn.input_[k][j] = 0.0;
	}

	for (j = 0; j < nn.size_output[nn.layers_count-1]; j++)
		output.push_back(0.0);

	nn_output_limit = 1.0;


	/*
	k = 1;
	for (j = 0; j < nn.size_output[k]; j++)
	{
		for (i = 0; i < nn.size_input_[k]; i++)
			printf("%6.3f ", nn.w[k][j][i]);
		printf("\n");
	}

	while (1)
		sleep(1);

	*/

}

CNeuralNetwork::~CNeuralNetwork()
{
	u32 j, k;
	output.clear();

	for (k = 0; k < nn.layers_count; k++)
	{
		for (j = 0; j < nn.size_output[k]; j++)
		{
			free(nn.w[k][j]);
			nn.w[k][j] = NULL;

			free(nn.dw[k][j]);
			nn.dw[k][j] = NULL;
		}

		free(nn.w[k]);
		nn.w[k] = NULL;

		free(nn.dw[k]);
		nn.dw[k] = NULL;

		free(nn.output[k]);
		nn.output[k] = NULL;

		free(nn.error[k]);
		nn.error[k] = NULL;

		free(nn.input[k]);
		nn.input[k] = NULL;
	}

	free(nn.w);
	nn.w = NULL;

	free(nn.dw);
	nn.dw = NULL;

	free(nn.output);
	nn.output = NULL;

	free(nn.error);
	nn.error = NULL;

	free(nn.input);
	nn.input = NULL;
	// printf("destructor done\n");
}

void CNeuralNetwork::process(std::vector<float> input)
{
	u32 i, j, k;

	for (i = 0; i < nn.size_input[0]; i++)
		nn.input[0][i] = input[i];

	for (k = 0; k < nn.layers_count; k++)
	{
		u32 ptr = 0;
		for (j = 0; j < nn.size_input[k]; j++)
		{
			nn.input_[k][ptr] = nn.input[k][j];
			ptr++;
		}

		if (nn.neuron_type == NEURON_TYPE_MIXED)
		{
			for (j = 0; j < nn.size_input[k]; j++)
			for (i = j; i < nn.size_input[k]; i++)
			{
				nn.input_[k][ptr] = nn.input[k][j]*nn.input[k][i];
				ptr++;
			}

			for (j = 3; j < nn.order; j++)
			for (i = 0; i < nn.size_input[k]; i++)
			{
				nn.input_[k][ptr] = pow(nn.input[k][i], j);
				ptr++;
			}
		}

		for (j = 0; j < nn.size_output[k]; j++)
		{
			float sum = 0.0;
			for (i = 0; i < nn.size_input_[k]; i++)
				sum+= nn.input_[k][i]*nn.w[k][j][i];

			if (nn.neuron_type == NEURON_TYPE_COMMON)
				sum = tanh(sum);


			if (sum > nn_output_limit)
				sum = nn_output_limit;

			if (sum < -nn_output_limit)
				sum = -nn_output_limit;

			nn.output[k][j] = sum;
		}

		if (k != (nn.layers_count-1))
		{
			for (i = 0; i < nn.size_output[k]; i++)
				nn.input[k+1][i] = nn.output[k][i];
		}
	}

	k = nn.layers_count-1;
	for (i = 0; i < nn.size_output[k]; i++)
		output[i] = nn.output[k][i];
}

std::vector<float> CNeuralNetwork::get()
{
	return output;
}

void CNeuralNetwork::learn(std::vector<float> required_output)
{
	u32 i, j, k, k_;

	float lc = nn.learning_constant;
	float weight_range = nn.weight_range;

	for (i = 0; i < nn.size_output[nn.layers_count - 1]; i++)
		nn.error[nn.layers_count-1][i]  = required_output[i] - nn.output[nn.layers_count-1][i];

	for (k_ = 0; k_ < nn.layers_count; k_++)
	//k_ = 0;
	{
		k = nn.layers_count - k_ - 1;

		for (j = 0; j < nn.size_output[k]; j++)
		{
			for (i = 0; i < nn.size_input_[k]; i++)
			{
				float dw = lc*nn.input_[k][i]*nn.error[k][j];
				float w =  nn.w[k][j][i] + dw;

				// printf("%f ", dw);

				if (w > weight_range)
					w = weight_range;

				if (w < -weight_range)
					w = -weight_range;

				nn.dw[k][j][i] = dw;
				nn.w[k][j][i] = w;
			}
		}

		if (k > 0)
		{
			for (j = 0; j < nn.size_output[k-1]; j++)
				nn.error[k-1][j] = 0.0;

			u32 ptr = 0;
			for (j = 0; j < nn.size_output[k-1]; j++)
			{
				for (i = 0; i < nn.size_output[k]; i++)
				{
					nn.error[k-1][j]+= nn.w[k][i][ptr]*nn.error[k][i];
					ptr++;
				}

				/*
				u32 o;
				for (o = 3; o < nn.order; o++)
				for (i = 0; i < nn.size_output[k]; i++)
				{
					nn.error[k-1][j]+= nn.w[k][i][ptr]*pow(nn.error[k][i], o);
					ptr++;
				}
				*/
			}
		}
	}
}

float*** CNeuralNetwork::get_weights()
{
	return nn.w;
}

float*** CNeuralNetwork::get_dweights()
{
	return nn.dw;
}


void CNeuralNetwork::set_weights(float ***w)
{
	merge_weights(w, 0.0);
}

void CNeuralNetwork::merge_weights(float ***w, float weight)
{
	if (weight < 0.0)
		weight = 0.0;

	if (weight > 1.0)
		weight = 1.0;

	u32 i, j, k;
	for (k = 0; k < nn.layers_count; k++)
		for (j = 0; j < nn.size_output[k]; j++)
			for (i = 0; i < nn.size_input_[k]; i++)
				nn.w[k][j][i] = weight*nn.w[k][j][i] + (1.0 - weight)*w[k][j][i];

}


void CNeuralNetwork::merge_dweights(float ***dw)
{
	u32 i, j, k;
	for (k = 0; k < nn.layers_count; k++)
		for (j = 0; j < nn.size_output[k]; j++)
			for (i = 0; i < nn.size_input_[k]; i++)
				nn.w[k][j][i]+= dw[k][j][i];

}

float CNeuralNetwork::rnd()
{
	return ((rand()%2000000) - 1000000)/1000000.0;
}


void CNeuralNetwork::set_learning_constant(float learning_constant)
{
	nn.learning_constant = learning_constant;
}

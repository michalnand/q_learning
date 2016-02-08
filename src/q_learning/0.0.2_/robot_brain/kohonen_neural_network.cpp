#include "kohonen_neural_network.h"

/*
class CKohonenNeuralNetwork
{
	private:
        struct sKohonenNeuralNetworkInitStructure nn_init;

		std::vector<float> input;
		std::vector<float> output;

        float **w;

        u32 winning_neuron;

	public:
*/

CKohonenNeuralNetwork::CKohonenNeuralNetwork(struct sKohonenNeuralNetworkInitStructure nn_init_structure)
{
    nn_init = nn_init_structure;

    u32 j, i;

    for (j = 0; j < nn_init.input_size; j++)
        input.push_back(0.0);

    w = (float**)malloc(nn_init.output_size*sizeof(float*));

    for (j = 0; j < nn_init.output_size; j++)
    {
        w[j] = (float*)malloc(nn_init.input_size*sizeof(float));

        for (i = 0; i < nn_init.input_size; i++)
            w[j][i] = rnd()*nn_init.weight_range*nn_init.weight_range_init;

        output.push_back(0.0);
    }
}


CKohonenNeuralNetwork::~CKohonenNeuralNetwork()
{
    u32 j;

    for (j = 0; j < nn_init.output_size; j++)
    {
        free(w[j]);
        w[j] = NULL;
    }

    free(w);

    w = NULL;
}

void CKohonenNeuralNetwork::process(std::vector<float> input)
{
    this->input = input;

    u32 j, i;

    float output_max = 0.0000000000000000001;

    //calculate output, and find winning_neuron
    for (j = 0; j < nn_init.output_size; j++)
    {
        float dist = 0.0;
        for (i = 0; i < nn_init.input_size; i++)
            dist+= pow(this->input[i] - w[j][i], 2.0);

        output[j] = dist;

        if (output_max < output[j])
            output_max = output[j];

    }

    winning_neuron = 0;

    //normalise output into <0, 1> range
    for (j = 0; j < nn_init.output_size; j++)
    {
        output[j] = output[j] / output_max;

        //reverse output, higher value = higher match
        output[j] = 1.0 - output[j];

        if (output[j] > output[winning_neuron])
            winning_neuron = j;
    }

    //learn network
    for (j = 0; j < nn_init.output_size; j++)
    {
        float k = 0.0;

        if (j == winning_neuron)
            k = nn_init.learning_constant;
        else
            k = nn_init.learning_constant*output[j]*0.01;

        for (i = 0; i < nn_init.input_size; i++)
            w[j][i] = (1.0 - k)*w[j][i] + k*this->input[i];
    }

    //for (j = 0; j < nn_init.output_size; j++)
    //    output[j] = (output[j] - 0.5)*2.0;
}

void CKohonenNeuralNetwork::process_without_learn(std::vector<float> input)
{
    this->input = input;

    u32 j, i;

    float output_max = 0.0000000000000000001;

    //calculate output, and find winning_neuron
    for (j = 0; j < nn_init.output_size; j++)
    {
        float dist = 0.0;
        for (i = 0; i < nn_init.input_size; i++)
            dist+= pow(this->input[i] - w[j][i], 2.0);

        output[j] = dist;

        if (output_max < output[j])
            output_max = output[j];

    }

    winning_neuron = 0;

    //normalise output into <0, 1> range
    for (j = 0; j < nn_init.output_size; j++)
    {
        output[j] = output[j] / output_max;

        //reverse output, higher value = higher match
        output[j] = 1.0 - output[j];

        if (output[j] > output[winning_neuron])
            winning_neuron = j;
    }

    //for (j = 0; j < nn_init.output_size; j++)
    //    output[j] = (output[j] - 0.5)*2.0;
}

std::vector<float> CKohonenNeuralNetwork::get()
{
    return output;
}

float** CKohonenNeuralNetwork::get_weights()
{
    return NULL;
}

float** CKohonenNeuralNetwork::get_dweights()
{
    return NULL;
}

void CKohonenNeuralNetwork::set_weights(float **w)
{

}

void CKohonenNeuralNetwork::set_weight(u32 i, u32 j, float value)
{

}

void CKohonenNeuralNetwork::merge_weights(float **w, float weight)
{

}

void CKohonenNeuralNetwork:: merge_dweights(float **dw)
{

}

void CKohonenNeuralNetwork::set_learning_constant(float learning_constant)
{
    nn_init.learning_constant = learning_constant;
}

u32 CKohonenNeuralNetwork::get_winning_neuron()
{
    return winning_neuron;
}

float CKohonenNeuralNetwork::rnd()
{
    return ((rand()%2000000) - 1000000)/1000000.0;
}

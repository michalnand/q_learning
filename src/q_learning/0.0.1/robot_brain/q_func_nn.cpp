#include "q_func_nn.h"

CQFuncNN::CQFuncNN(u32 state_size, u32 action_size, float state_density,
              float action_density, float alpha)
{
    this->alpha = alpha;

    struct sNeuralNetworkInitStructure nn_init;

    //u32 neuron_type = NEURON_TYPE_COMMON;
	u32 neuron_type = NEURON_TYPE_MIXED;
	u32 hidden_neurons_count = 10;
    float eta = 0.001;

    NeuralNetworkInitStructure_init(&nn_init,
                                    4, 4.0, 7, neuron_type, eta, 0.1);

    //neural network input : state and action vector size, +1 for bias
    nn_init.init_vector[0] = state_size + action_size + 1;
    nn_init.init_vector[1] = hidden_neurons_count;
    nn_init.init_vector[2] = hidden_neurons_count;
    nn_init.init_vector[3] = 1;

    u32 i;
    for (i = 0; i < nn_init.init_vector[0]; i++)
        nn_input.push_back(0.0);

    nn = new CNeuralNetwork(nn_init);

    NeuralNetworkInitStructure_uninit(&nn_init);
}

CQFuncNN::~CQFuncNN()
{
    delete nn;
}

float CQFuncNN::get(std::vector<float> state, std::vector<float> action)
{
    u32 ptr = 0, i;

    for (i = 0; i < state.size(); i++)
        nn_input[ptr++] = state[i];

    for (i = 0; i < action.size(); i++)
        nn_input[ptr++] = action[i];

    nn_input[ptr++] = 1.0;

    nn->process(nn_input);

    return nn->get()[0];
}

void CQFuncNN::learn(std::vector<float> state, std::vector<float> action, float required_value)
{
    std::vector<float> required_value_;

    required_value_.push_back(tanh(1.3*required_value));

    u32 ptr = 0, i;

    for (i = 0; i < state.size(); i++)
        nn_input[ptr++] = state[i];

    for (i = 0; i < action.size(); i++)
        nn_input[ptr++] = action[i];

    nn_input[ptr++] = 1.0;

    nn->process(nn_input);
    nn->learn(required_value_);
}

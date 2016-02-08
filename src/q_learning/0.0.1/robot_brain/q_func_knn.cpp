#include "q_func_knn.h"

CQFuncKNN::CQFuncKNN(u32 state_size, u32 action_size, float state_density,
              float action_density, float alpha)
{
    this->alpha = alpha;

    struct sKohonenNeuralNetworkInitStructure knn_init;

    knn_init.input_size = state_size;
    knn_init.output_size = 16;
    knn_init.weight_range = 1.0;
    knn_init.learning_constant = 0.001;
    knn_init.weight_range_init = 1.0;

    knn = new CKohonenNeuralNetwork(knn_init);


    struct sNeuralNetworkInitStructure nn_init;

    //u32 neuron_type = NEURON_TYPE_COMMON;
	u32 neuron_type = NEURON_TYPE_MIXED;
	u32 hidden_neurons_count = 8;
    float eta = 0.001;

    NeuralNetworkInitStructure_init(&nn_init,
                                    3, 1.0, 3, neuron_type, eta, 0.1);

    //neural network input : state and action vector size, +1 for bias
    nn_init.init_vector[0] = knn_init.output_size + action_size + 1;
    nn_init.init_vector[1] = hidden_neurons_count;
    nn_init.init_vector[2] = 1;

    u32 i;
    for (i = 0; i < nn_init.init_vector[0]; i++)
        nn_input.push_back(0.0);

    nn = new CNeuralNetwork(nn_init);

    NeuralNetworkInitStructure_uninit(&nn_init);
}

CQFuncKNN::~CQFuncKNN()
{
    delete nn;
    delete knn;
}

float CQFuncKNN::get(std::vector<float> state, std::vector<float> action)
{
    u32 ptr = 0, i;

    //map state into kohonen network
    knn->process(state);

    u32 knn_output_size = knn->get().size();
    //kohonen output as input into fnn
    for (i = 0; i < knn_output_size; i++)
        nn_input[ptr++] = knn->get()[i];

    for (i = 0; i < action.size(); i++)
        nn_input[ptr++] = action[i];

    //bias
    nn_input[ptr++] = 1.0;

    nn->process(nn_input);

    return nn->get()[0];
}

void CQFuncKNN::learn(std::vector<float> state, std::vector<float> action, float required_value)
{
    std::vector<float> required_value_;

    required_value_.push_back(tanh(1.3*required_value));

    u32 ptr = 0, i;

    knn->process_without_learn(state);

    u32 knn_output_size = knn->get().size();
    //kohonen output as input into fnn
    for (i = 0; i < knn_output_size; i++)
        nn_input[ptr++] = knn->get()[i];

    for (i = 0; i < action.size(); i++)
        nn_input[ptr++] = action[i];

    nn_input[ptr++] = 1.0;

    nn->process(nn_input);
    nn->learn(required_value_);
}

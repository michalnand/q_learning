#include "q_function.h"

CQfunction::CQfunction(u32 state_vector_size, u32 action_vector_size, float alpha)
{
    this->state_vector_size = state_vector_size;
    this->action_vector_size = action_vector_size;
    this->alpha = alpha;

    state_idx = 0;
    action_idx = 0;

    u32 state_neurons_count = 16;
    u32 action_neurons_count = 16;
    float eta = 0.01;

    struct sKohonenNeuralNetworkInitStructure knn_init;

    knn_init.input_size = state_vector_size;
    knn_init.output_size = state_neurons_count;
    knn_init.weight_range = 1.0;
    knn_init.learning_constant = eta;
    knn_init.weight_range_init = 1.0;
    state_associative_array = new CKohonenNeuralNetwork(knn_init);

    knn_init.input_size = action_vector_size;
    knn_init.output_size = action_neurons_count;
    knn_init.weight_range = 1.0;
    knn_init.learning_constant = eta;
    knn_init.weight_range_init = 1.0;
    action_associative_array = new CKohonenNeuralNetwork(knn_init);

    u32 i, j;

    for (j = 0; j < state_neurons_count; j++)
    {
        std::vector<float> tmp;
        for (i = 0; i < action_neurons_count; i++)
            tmp.push_back(0.0);

        q.push_back(tmp);
    }
}

CQfunction::~CQfunction()
{
    delete state_associative_array;
    delete action_associative_array;
}

float CQfunction::get(std::vector<float> state, std::vector<float> action)
{
    state_associative_array->process(state);
    state_idx = state_associative_array->get_winning_neuron();

    action_associative_array->process(action);
    action_idx = action_associative_array->get_winning_neuron();

    return q[state_idx][action_idx];
}

void CQfunction::learn(std::vector<float> state, std::vector<float> action, float value)
{
    state_associative_array->process(state);
    state_idx = state_associative_array->get_winning_neuron();

    action_associative_array->process(action);
    action_idx = action_associative_array->get_winning_neuron();

    q[state_idx][action_idx] = alpha*q[state_idx][action_idx] + (1.0 - alpha)*value;
}

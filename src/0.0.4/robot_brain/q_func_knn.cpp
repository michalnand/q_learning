#include "q_func_knn.h"



CQFuncKNN::CQFuncKNN(u32 state_size, u32 action_size, float state_density,
              float action_density, float alpha)
{
    this->alpha = alpha;


    knn_init.inputs_count = state_size + action_size;
    knn_init.neurons_count = 256*4;
    knn_init.weight_range = 1.0;
    knn_init.learning_constant = 0.001;
    knn_init.output_limit = 1.0;

    knn = new CKohonenLayer(knn_init);

    u32 i;
    for (i = 0; i < knn_init.inputs_count; i++)
      nn_input.push_back(0.0);

    for (i = 0; i < knn_init.neurons_count; i++)
      nn_output.push_back(0.0);
}

CQFuncKNN::~CQFuncKNN()
{
    delete knn;
}

float CQFuncKNN::get(std::vector<float> state, std::vector<float> action)
{
    u32 ptr = 0, i;

    for (i = 0; i < state.size(); i++)
      nn_input[ptr++] = state[i];

    for (i = 0; i < action.size(); i++)
      nn_input[ptr++] = action[i];

    //map state into kohonen network
    knn->process(nn_input);

    u32 winning_neuron_idx = knn->get_winning_neuron_idx();

    return nn_output[winning_neuron_idx];
}

void CQFuncKNN::learn(std::vector<float> state, std::vector<float> action, float required_value)
{
  u32 ptr = 0, i;

  for (i = 0; i < state.size(); i++)
    nn_input[ptr++] = state[i];

  for (i = 0; i < action.size(); i++)
    nn_input[ptr++] = action[i];

  nn_input[ptr++] = required_value;

  //map state into kohonen network
  knn->process(nn_input);

  u32 winning_neuron_idx = knn->get_winning_neuron_idx();

  knn->learn();

  float k = knn_init.learning_constant;

  nn_output[winning_neuron_idx] = (1.0 - k)*nn_output[winning_neuron_idx] + k*required_value;
}

i32 CQFuncKNN::save(char *file_name)
{
    return -1;
}

i32 CQFuncKNN::load(char *file_name)
{
    return -1;
}

void CQFuncKNN::merge(class CQFuncKNN *q_func)
{
  //TODO
}

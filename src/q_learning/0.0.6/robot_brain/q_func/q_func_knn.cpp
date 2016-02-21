#include "q_func_knn.h"



CQFuncKNN::CQFuncKNN(u32 state_size, u32 action_size, float state_density,
              float action_density, float alpha, u32 neuron_type, std::vector<std::vector<float>> actions)
{
    this->alpha = alpha;
    this->actions = actions;

    u32 i;
    std::vector<u32> init_vector;

    for (i = 0; i < state_size; i++)
    {
      init_vector.push_back(ceil(0.5*1.0/state_density));
      q_idx_f.push_back(0.0);
    }

    for (i = 0; i < actions.size(); i++)
      q_tables.push_back(new CArray(init_vector));


    u32 hidden_neurons_count = 4;
    init_vector.clear();
    init_vector.push_back(2*state_size + 1 + 1);
    init_vector.push_back(hidden_neurons_count);
    init_vector.push_back(1);

    struct sNNInitStruct nn_init;

    nn_init.init_vector = init_vector;

    nn_init.order = 5;
    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;

    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_INTERSYNAPTICS;
    //nn_init.neuron_type = neuron_type;

    nn_init.weight_range = 4.0;
    nn_init.init_weight_range = 0.3*nn_init.weight_range;
    nn_init.learning_constant = 1.0/1000.0;
    nn_init.output_limit = 4.0;

    for (i = 0; i < actions.size(); i++)
        nn.push_back( new CNN(nn_init) );

    for (i = 0; i < (state_size + 1); i++)
      bf_nn_input.push_back(0.0);


    for (i = 0; i < actions.size(); i++)
    {
      u32 count = 16;
      u32 dimension =  bf_nn_input.size();
      float a_range = 1.0;
      float b_range = 10.0;
      float w_range = 4.0;
      bf_nn.push_back( new CBasisFunctions(count, dimension, a_range, b_range, w_range));
    }
}

CQFuncKNN::~CQFuncKNN()
{
    u32 i;
    for (i = 0; i < q_tables.size(); i++)
      delete q_tables[i];

    for (i = 0; i < nn.size(); i++)
      delete nn[i];

    for (i = 0; i < bf_nn.size(); i++)
      delete bf_nn[i];
}

void CQFuncKNN::learn_start()
{

}

void CQFuncKNN::learn_finish()
{

}

u32 CQFuncKNN::get_action_idx(std::vector<float> action)
{
  u32 i, j;
  u32 action_idx = 0;
  float action_dist = 1000000000.0;

  for (j = 0; j < actions.size(); j++)
  {
    float dist = 0.0;
    for (i = 0; i < actions[j].size(); i++)
      dist+= abs_(actions[j][i] - action[i]);

    if (dist < action_dist)
    {
      action_dist = dist;
      action_idx = j;
    }
  }

  return action_idx;
}

float CQFuncKNN::get(std::vector<float> state, std::vector<float> action)
{
    u32 i;
    u32 ptr = 0;
    u32 action_idx = get_action_idx(action);

    for (i = 0; i < state.size(); i++)
    {
      q_idx_f[ptr] = (state[i] + 1.0)/2.0;
      ptr++;
    }

    float q_aprox = q_tables[action_idx]->get_f(q_idx_f);
    std::vector<float> residuum = q_tables[action_idx]->get_residuum();

    bf_nn_input[0] = residuum[0];
    bf_nn_input[1] = residuum[1];
    bf_nn_input[2] = q_aprox;

    bf_nn[action_idx]->process_linear_combination(bf_nn_input);
    float bf_res = bf_nn[action_idx]->get_linear_combination();

    return bf_res + q_aprox;
}

void CQFuncKNN::learn(std::vector<float> state, std::vector<float> action, float required_value)
{
    u32 i;
    u32 ptr = 0;
    u32 action_idx = get_action_idx(action);

    for (i = 0; i < state.size(); i++)
    {
      q_idx_f[ptr] = (state[i] + 1.0)/2.0;
      ptr++;
    }

    float tmp = alpha*q_tables[action_idx]->get_f(q_idx_f) + (1.0 - alpha)*required_value;
    q_tables[action_idx]->set_f(q_idx_f, tmp);

    float q_aprox = q_tables[action_idx]->get_f(q_idx_f);
    std::vector<float> residuum = q_tables[action_idx]->get_residuum();

    bf_nn_input[0] = residuum[0];
    bf_nn_input[1] = residuum[1];
    bf_nn_input[2] = q_aprox;

    bf_nn[action_idx]->process_linear_combination(bf_nn_input);
    float bf_res = bf_nn[action_idx]->get_linear_combination();

    bf_nn[action_idx]->learn_linear_combination(required_value - q_aprox, 0.01);
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

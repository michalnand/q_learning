#include "q_func_bfnn.h"



CQFuncBFNN::CQFuncBFNN(u32 state_size, u32 action_size, float state_density,
              float action_density, float alpha, u32 topology_type,  u32 function_type, std::vector<std::vector<float>> actions)
{
    this->alpha = alpha;
    this->actions = actions;
    this->topology_type = topology_type;
    this->function_type = function_type;

    #ifdef DEBUG_MODE
    printf("  Q BFNN topology %u function %u\n", this->topology_type, this->function_type);
    #endif


    u32 i;
    std::vector<u32> init_vector;

    for (i = 0; i < state_size; i++)
    {
      init_vector.push_back(ceil(0.25*1.0/state_density));
      q_idx_f.push_back(0.0);
    }

    for (i = 0; i < actions.size(); i++)
      q_tables.push_back(new CArray(init_vector));


    u32 bf_nn_count = 64;
    u32 bf_nn_dimension =  state_size;
    float a_range = 1.0;
    float b_range = 200.0;
    float w_range = 4.0;

    for (i = 0; i < actions.size(); i++)
        bf_nn.push_back( new CBasisFunctions(bf_nn_count, bf_nn_dimension, a_range, b_range, w_range, this->function_type));

    u32 hidden_neurons_count = 4;
    init_vector.clear();
    init_vector.push_back(bf_nn_count + 1);
    init_vector.push_back(hidden_neurons_count);
    init_vector.push_back(1);
}

CQFuncBFNN::~CQFuncBFNN()
{
    u32 i;
    for (i = 0; i < q_tables.size(); i++)
      delete q_tables[i];

    for (i = 0; i < bf_nn.size(); i++)
      delete bf_nn[i];
}

void CQFuncBFNN::learn_start()
{

}

void CQFuncBFNN::learn_finish()
{

}

u32 CQFuncBFNN::get_action_idx(std::vector<float> action)
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

float CQFuncBFNN::get(std::vector<float> state, std::vector<float> action)
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

    bf_nn[action_idx]->process_linear_combination(state);
    float bf_res = bf_nn[action_idx]->get_linear_combination();

    float k = 1.0;
    if (topology_type == BFNN_TOPOLOGY_TYPE_PURE)
      k = 0.0;


    return bf_res + k*q_aprox;
}

void CQFuncBFNN::learn(std::vector<float> state, std::vector<float> action, float required_value, float reward_value)
{
    u32 i;
    u32 ptr = 0;
    u32 action_idx = get_action_idx(action);

    for (i = 0; i < state.size(); i++)
    {
      q_idx_f[ptr] = (state[i] + 1.0)/2.0;
      ptr++;
    }

    float alpha = 0.98;
    float tmp = alpha*q_tables[action_idx]->get_f(q_idx_f) + (1.0 - alpha)*required_value;
    q_tables[action_idx]->set_f(q_idx_f, tmp);

    float q_aprox = q_tables[action_idx]->get_f(q_idx_f);
    bf_nn[action_idx]->process_linear_combination(state);


    float k = 1.0;
    if (topology_type == BFNN_TOPOLOGY_TYPE_PURE)
      k = 0.0;

    float required_value_ = required_value - k*q_aprox;

    bf_nn[action_idx]->learn_linear_combination(required_value_, 0.01, true, reward_value);
}

i32 CQFuncBFNN::save(char *file_name)
{
    u32 i;
    for (i = 0; i < bf_nn.size(); i++)
      bf_nn[i]->save(i);

    return -1;
}

i32 CQFuncBFNN::load(char *file_name)
{
    return -1;
}

void CQFuncBFNN::merge(class CQFuncBFNN *q_func)
{
  //TODO
}

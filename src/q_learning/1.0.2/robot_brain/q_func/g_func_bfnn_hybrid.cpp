#include "q_func_bfnn_hybrid.h"


CQFuncBFNNHybrid::CQFuncBFNNHybrid(u32 state_size, std::vector<std::vector<float>> actions)
{
    u32 i; 

    this->actions = actions;
    this->learning_rate = 0.01;

    u32 count_peek = 64;
    u32 count_hill = 8;
    u32 dimension = state_size;

    for (i = 0; i < actions.size(); i++)
      bf_nn.push_back( new CBasisFunctionsHybrid(count_peek, count_hill, dimension) );
}

CQFuncBFNNHybrid::~CQFuncBFNNHybrid()
{
    u32 i;
    for (i = 0; i < bf_nn.size(); i++)
      delete bf_nn[i];
}


u32 CQFuncBFNNHybrid::get_action_idx(std::vector<float> action)
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

float CQFuncBFNNHybrid::get(std::vector<float> state, std::vector<float> action)
{
    u32 action_idx = get_action_idx(action);
    bf_nn[action_idx]->process(state);
    float bf_res = bf_nn[action_idx]->get();

    return bf_res;
}

void CQFuncBFNNHybrid::learn(std::vector<float> state, std::vector<float> action, float required_value)
{
    u32 action_idx = get_action_idx(action);

    bf_nn[action_idx]->process(state);
    bf_nn[action_idx]->learn(required_value, learning_rate);
}

i32 CQFuncBFNNHybrid::save(char *file_name)
{
    return -1;
}

i32 CQFuncBFNNHybrid::load(char *file_name)
{
    return -1;
}

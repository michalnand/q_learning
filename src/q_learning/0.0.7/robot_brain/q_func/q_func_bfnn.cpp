#include "q_func_bfnn.h"



CQFuncBFNN::CQFuncBFNN(u32 state_size, u32 action_size, float state_density,
              float action_density, float alpha, u32 network_type, std::vector<std::vector<float>> actions)
{
    this->alpha = alpha;
    this->actions = actions;
    this->network_type = network_type;

    u32 i;
    std::vector<u32> init_vector;

    for (i = 0; i < state_size; i++)
    {
      init_vector.push_back(ceil(0.5*1.0/state_density));
      q_idx_f.push_back(0.0);
    }

    for (i = 0; i < actions.size(); i++)
      q_tables.push_back(new CArray(init_vector));


    for (i = 0; i < (state_size ); i++)
      bf_nn_input.push_back(0.0);

    u32 bf_nn_count = 64;
    u32 bf_nn_dimension =  bf_nn_input.size();
    float a_range = 1.0;
    float b_range = 100.0;
    float w_range = 4.0;

    for (i = 0; i < actions.size(); i++)
      bf_nn.push_back( new CBasisFunctions(bf_nn_count, bf_nn_dimension, a_range, b_range, w_range));




    u32 hidden_neurons_count = 4;
    init_vector.clear();
    init_vector.push_back(bf_nn_count + 1);
    //init_vector.push_back(hidden_neurons_count);
    init_vector.push_back(1);

    struct sNNInitStruct nn_init;

    nn_init.init_vector = init_vector;

    nn_init.order = 5;

    nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;
    switch (network_type)
    {
      case BFNN_TANH:           nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;
      case BFNN_INTERSYNAPTICS: nn_init.neuron_type = NN_LAYER_NEURON_TYPE_INTERSYNAPTICS;
    }

    nn_init.weight_range = 4.0;
    nn_init.init_weight_range = 0.001; //0.3*nn_init.weight_range;
    nn_init.learning_constant = 1.0/100.0;
    nn_init.output_limit = 4.0;

    for (i = 0; i < actions.size(); i++)
        nn.push_back( new CNN(nn_init) );

}

CQFuncBFNN::~CQFuncBFNN()
{
    u32 i;
    for (i = 0; i < q_tables.size(); i++)
      delete q_tables[i];

    for (i = 0; i < bf_nn.size(); i++)
      delete bf_nn[i];

    for (i = 0; i < actions.size(); i++)
      delete nn[i];
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
    float res = 0.0;

    for (i = 0; i < state.size(); i++)
    {
      q_idx_f[ptr] = (state[i] + 1.0)/2.0;
      ptr++;
    }

    float q_aprox = q_tables[action_idx]->get_f(q_idx_f);


    bf_nn[action_idx]->process_linear_combination(state);
    float bf_res = bf_nn[action_idx]->get_linear_combination();

    std::vector<float> nn_input = bf_nn[action_idx]->get();
    nn_input.push_back(1.0);

    switch (network_type)
    {
      case BFNN_TANH:           nn[action_idx]->process(nn_input);
                                res = nn[action_idx]->get()[0];
                                break;

      case BFNN_INTERSYNAPTICS: nn[action_idx]->process(nn_input);
                                res = nn[action_idx]->get()[0];
                                break;

      default:
                                res = bf_res;
                                break;
    }

    return res + q_aprox;
}

void CQFuncBFNN::learn(std::vector<float> state, std::vector<float> action, float required_value)
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


    bf_nn[action_idx]->process_linear_combination(state);

    std::vector<float> nn_input = bf_nn[action_idx]->get();
    nn_input.push_back(1.0);

    std::vector<float> required_value_;

    required_value_.push_back(required_value - q_aprox);

    switch (network_type)
    {
      case BFNN_TANH:
                                bf_nn[action_idx]->learn_linear_combination(required_value_[0], 0.01, true);
                                nn[action_idx]->process(nn_input);
                                nn[action_idx]->learn(required_value_);
                                break;

      case BFNN_INTERSYNAPTICS:
                                bf_nn[action_idx]->learn_linear_combination(required_value_[0], 0.01, true);
                                nn[action_idx]->process(nn_input);
                                nn[action_idx]->learn(required_value_);
                                break;

      default:
                                bf_nn[action_idx]->learn_linear_combination(required_value_[0], 0.01, true);
                                break;
    }
}

i32 CQFuncBFNN::save(char *file_name)
{
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

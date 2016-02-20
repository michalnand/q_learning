#include "q_func_nn.h"

CQFuncNN::CQFuncNN(u32 state_size, u32 action_size, float state_density,
              float action_density, float alpha, u32 neuron_type, u32 actions_count)
{
    u32 i;
    for (i = 0; i < (state_size + action_size + 1); i++)
        nn_input.push_back(0.0);


    this->alpha = alpha;

    u32 hidden_neurons_count = 32;

    std::vector<u32> init_vector;
    init_vector.push_back(state_size + 0*action_size + 1);
    init_vector.push_back(hidden_neurons_count);
    init_vector.push_back(1);

    struct sNNInitStruct nn_init;

    nn_init.init_vector = init_vector;

    nn_init.order = 5;
    //nn_init.neuron_type = NN_LAYER_NEURON_TYPE_TANH;
    //nn_init.neuron_type = NN_LAYER_NEURON_TYPE_INTERSYNAPTICS;

    nn_init.neuron_type = neuron_type;

    nn_init.weight_range = 4.0;
    nn_init.init_weight_range = 0.3*nn_init.weight_range;
    nn_init.learning_constant = 1.0/1000.0;
    nn_init.output_limit = 4.0;

    for (i = 0; i < actions_count; i++)
        nn.push_back( new CNN(nn_init) );
}

CQFuncNN::~CQFuncNN()
{
  u32 i;
  for (i = 0; i < nn.size(); i++)
    delete nn[i];
}

float CQFuncNN::get(std::vector<float> state, std::vector<float> action, u32 action_id)
{
    u32 ptr = 0, i;

    for (i = 0; i < state.size(); i++)
        nn_input[ptr++] = state[i];

        /*
    float tmp = 0.0;
    for (i = 0; i < state.size(); i++)
      tmp+= state[i]*state[i];

    tmp = sqrt(tmp)/state.size();
    nn_input[ptr++] = tmp;
      */
    /*
    for (i = 0; i < action.size(); i++)
        nn_input[ptr++] = action[i];
    */

    nn_input[ptr++] = -1.0;
    nn[action_id]->process(nn_input);
    return nn[action_id]->get()[0];
}


void CQFuncNN::learn(std::vector<float> state, std::vector<float> action, float required_value, u32 action_id)
{
    std::vector<float> required_value_;

    required_value_.push_back(required_value);

    u32 ptr = 0, i;

    for (i = 0; i < state.size(); i++)
        nn_input[ptr++] = state[i];

        /*
    float tmp = 0.0;
    for (i = 0; i < state.size(); i++)
      tmp+= state[i]*state[i];

    tmp = sqrt(tmp)/state.size();
    nn_input[ptr++] = tmp;
      */

    /*
    for (i = 0; i < action.size(); i++)
        nn_input[ptr++] = action[i];
    */

    nn_input[ptr++] = -1.0;
    nn[action_id]->process(nn_input);
    nn[action_id]->learn(required_value_ );
}


i32 CQFuncNN::save(char *file_name)
{
    return -1;
}

i32 CQFuncNN::load(char *file_name)
{
    return -1;
}

void CQFuncNN::merge(class CQFuncNN *q_func)
{
  //TODO
}

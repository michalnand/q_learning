#include "q_func.h"

CQFunc::CQFunc(u32 state_size, u32 action_size, float state_density,
              float action_density, float alpha)
{
    u32 i, j;

    state_associative_array = new CAssociativeArray(state_size, state_density);
    action_associative_array = new CAssociativeArray(action_size, action_density);

    for (j = 0; j < state_associative_array->get_size(); j++)
    {
        std::vector<float> q_tmp;
        for (i = 0; i < action_associative_array->get_size(); i++)
            q_tmp.push_back(0.0);

        q_values.push_back(q_tmp);
    }

    this->alpha = alpha;
}

CQFunc::~CQFunc()
{
    delete state_associative_array;
    delete action_associative_array;
}


float CQFunc::get(std::vector<float> state, std::vector<float> action)
{
    u32 state_idx = state_associative_array->get(state);
    u32 action_idx = action_associative_array->get(action);

    return q_values[state_idx][action_idx];
}

void CQFunc::learn(std::vector<float> state, std::vector<float> action, float required_value)
{
    u32 state_idx = state_associative_array->get(state);
    u32 action_idx = action_associative_array->get(action);

    q_values[state_idx][action_idx] = alpha*q_values[state_idx][action_idx] + (1.0 - alpha)*required_value;
}

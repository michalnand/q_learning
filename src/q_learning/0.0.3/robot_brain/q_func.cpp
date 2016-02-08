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

    u32 i;

    for (i = 0; i < q_values.size(); i++)
        q_values[i].clear();

    q_values.clear();
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


i32 CQFunc::save(char *file_name)
{
    u32 j, i;
    FILE *f;
    f = fopen(file_name, "w");

    fprintf(f,"%u %u %u\n", Q_FUNC_MAGIC, q_values.size(), q_values[0].size());

    if (f == NULL)
        return -1;

    for (j = 0; j < q_values.size(); j++)
        for (i = 0; i < q_values[j].size(); i++)
        {
            fprintf(f,"%f ", q_values[j][i]);
        }

    fclose(f);

    return 1;
}

i32 CQFunc::load(char *file_name)
{
    u32 j, i;
    FILE *f;
    f = fopen(file_name, "r");

    u32 magic, x, y;

    i32 res;

    res = fscanf(f, "%u ", &magic);

    if (magic != Q_FUNC_MAGIC)
        return -1;

    res = fscanf(f, "%u %u", &y, &x);

    for (i = 0; i < q_values.size(); i++)
        q_values[i].clear();

    q_values.clear();

    for (j = 0; j < y; j++)
    {
        std::vector<float> tmp;
        for (i = 0; i < x; i++)
        {
            float tmp_f = 0.0;
            res = fscanf(f,"%f ", &tmp_f);
            tmp.push_back(tmp_f);
        }

        q_values.push_back(tmp);
    }

    fclose(f);
    return 1;
}

float CQFunc::get_q_value(u32 j, u32 i)
{
    return q_values[j][i];
}

void CQFunc::merge(class CQFunc *q_func)
{
    u32 j, i;

    for (j = 0; j < q_values.size(); j++)
        for (i = 0; i < q_values[j].size(); i++)
            q_values[j][i] = q_func->get_q_value(j, i);
}

#include "q_func.h"

CQFunc::CQFunc(u32 state_size, u32 action_size, float state_density,
              float action_density, float alpha)
{
    u32 i;

    for (i = 0; i < state_size; i++)
      init_vector.push_back(ceil(1.0/state_density));

    for (i = 0; i < action_size; i++)
      init_vector.push_back(ceil(1.0/action_density));

    q_values = new CArray(init_vector);

    for (i = 0; i < (state_size+action_size); i++)
      q_idx_f.push_back(0.0);

    this->alpha = alpha;

}

CQFunc::~CQFunc()
{
  delete q_values;
}


float CQFunc::get(std::vector<float> state, std::vector<float> action)
{
    u32 i;
    u32 ptr = 0;

    for (i = 0; i < state.size(); i++)
    {
      q_idx_f[ptr] = (state[i] + 1.0)/2.0;
      ptr++;
    }

    for (i = 0; i < action.size(); i++)
    {
      q_idx_f[ptr] = (action[i] + 1.0)/2.0;
      ptr++;
    }

    return q_values->get_f(q_idx_f);
}

void CQFunc::learn(std::vector<float> state, std::vector<float> action, float required_value)
{
    u32 i;
    u32 ptr = 0;

    for (i = 0; i < state.size(); i++)
    {
      q_idx_f[ptr] = (state[i] + 1.0)/2.0;
      ptr++;
    }

    for (i = 0; i < action.size(); i++)
    {
      q_idx_f[ptr] = (action[i] + 1.0)/2.0;
      ptr++;
    }

    float tmp = alpha*q_values->get_f(q_idx_f) + (1.0 - alpha)*required_value;

    q_values->set_f(q_idx_f, tmp);
}


i32 CQFunc::save(char *file_name)
{
    u32 j;
    FILE *f;
    f = fopen(file_name, "w");

    fprintf(f,"%u %u\n", Q_FUNC_MAGIC, q_values->get_size());

    if (f == NULL)
        return -1;

    for (j = 0; j < q_values->get_size(); j++)
      fprintf(f,"%f ", q_values->get_idx(j));

    fclose(f);

    return 1;
}

i32 CQFunc::load(char *file_name)
{
    u32 j;
    FILE *f;
    f = fopen(file_name, "r");

    u32 magic, size = 0;
    i32 res;

    res = fscanf(f, "%u ", &magic);

    if (magic != Q_FUNC_MAGIC)
        return -1;

    res = fscanf(f, "%u", &size);


    for (j = 0; j < size; j++)
    {
      float tmp = 0.0;
      res = fscanf(f,"%f ", &tmp);

      q_values->set_idx(j, tmp);
    }

    fclose(f);
    return 1;
}

CArray* CQFunc::get_q_values()
{
  return q_values;
}

void CQFunc::merge(class CQFunc *q_func)
{
  u32 j;
  for (j = 0; j < q_values->get_size(); j++)
  {
    float value = q_func->get_q_values()->get_idx(j);
    q_values->set_idx(j, value);
  }
}

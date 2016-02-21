#include "q_learning.h"

CQlearning::CQlearning(struct sQlearningInit ql_init_struct)
{
  u32 i;

  this->q_init = ql_init_struct;
  q_res.reward = 0.0;

  for (i = 0; i < q_init.state_vector_size; i++)
    q_res.state.push_back(0.0);

  for (i = 0; i < q_init.action_vector_size; i++)
  {
    q_res.action.push_back(0.0);
    q_res.action_best.push_back(0.0);
  }

  q_res.action_id = 0;
  q_res.action_best_id = 0;
  q_res.q_value = 0.0;
  q_res.q_value_best = 0.0;

  for (i = 0; i < 100; i++)
    q_res_array.push_back(q_res);

  q_res_array_ptr = q_res_array.size() - 1;

  q_func = NULL;
  q_func_nn_mcp = NULL;
  q_func_nn_tn = NULL;
  q_func_nn_knn = NULL;

  q_func = new  CQFunc(
                        q_init.state_vector_size, q_init.action_vector_size,
                        q_init.density, q_init.density,
                        q_init.alpha
                      );


  q_func_nn_mcp = new  CQFuncNN(
                                q_init.state_vector_size, q_init.action_vector_size,
                                q_init.density, q_init.density,
                                q_init.alpha, NN_LAYER_NEURON_TYPE_TANH,
                                q_init.actions_count
                                );

  q_func_nn_tn = new  CQFuncNN(
                                q_init.state_vector_size, q_init.action_vector_size,
                                q_init.density, q_init.density,
                                q_init.alpha, NN_LAYER_NEURON_TYPE_INTERSYNAPTICS,
                                q_init.actions_count
                              );

  q_func_nn_knn = new CQFuncKNN(
                              q_init.state_vector_size, q_init.action_vector_size,
                              q_init.density, q_init.density,
                              q_init.alpha, NN_LAYER_NEURON_TYPE_TANH, //NN_LAYER_NEURON_TYPE_INTERSYNAPTICS,
                              q_init.actions
                            );

  for (i = 0; i < q_init.actions_count; i++)
    actions_score.push_back(0.0);


  #ifdef DEBUG_MODE
  printf(" NEW Qlearning : \n");
  printf("  state vector size %u \n", q_init.state_vector_size);
  printf("  action vector size %u \n", q_init.action_vector_size);
  printf("  actions count %u \n", q_init.actions_count);
  printf("  %6.3f %6.3f %6.3f %u\n", q_init.gamma, q_init.alpha, q_init.k, q_init.function_type);
  #endif
}

CQlearning::~CQlearning()
{
  if (q_func != NULL)
    delete q_func;

  if (q_func_nn_mcp != NULL)
    delete q_func_nn_mcp;

  if (q_func_nn_tn != NULL)
    delete q_func_nn_tn;

  if (q_func_nn_knn != NULL)
    delete q_func_nn_knn;
}

float CQlearning::get_highest_q(std::vector<float> state, std::vector<std::vector<float>> actions)
{
  u32 j;

  float q_res = -1000000000.0;
  for (j = 0; j < q_init.actions_count; j++)
  {
    float q_tmp =  0.0;

    switch (q_init.function_type)
    {
      case 0: q_tmp = q_func->get(state, actions[j]); break;
      case 1: q_tmp = q_func_nn_mcp->get(state, actions[j], j); break;
      case 2: q_tmp = q_func_nn_tn->get(state, actions[j], j); break;
      case 3: q_tmp = q_func_nn_knn->get(state, actions[j]); break;
    }
    if (q_tmp > q_res)
      q_res = q_tmp;
  }

  return q_res;
}

void CQlearning::process(std::vector<float> state, std::vector<std::vector<float>> actions, float reward,
                        u32 learn, u32 force_learn)
{
  q_res.state = state;
  q_res.reward = reward;

  u32 i, j;

  for (j = 0; j < actions.size(); j++)
  switch (q_init.function_type)
  {
    case 0:
            for (j = 0; j < q_init.actions_count; j++)
              actions_score[j] = q_func->get(q_res.state, actions[j]);
            break;

    case 1:
            for (j = 0; j < q_init.actions_count; j++)
              actions_score[j] = q_func_nn_mcp->get(q_res.state, actions[j], j);
            break;

    case 2:
            for (j = 0; j < q_init.actions_count; j++)
              actions_score[j] = q_func_nn_tn->get(q_res.state, actions[j], j);
            break;

    case 3:
            for (j = 0; j < q_init.actions_count; j++)
              actions_score[j] = q_func_nn_knn->get(q_res.state, actions[j]);
            break;
  }

  q_res.action_id = rand()%q_init.actions_count;
  q_res.action = actions[q_res.action_id];
  q_res.q_value = actions_score[q_res.action_id];

  //find best action
  q_res.action_best_id = 0;
  for (j = 0; j < q_init.actions_count; j++)
    if (actions_score[j] > actions_score[q_res.action_best_id])
    {
        q_res.action_best_id = j;
    }

  q_res.action_best = actions[q_res.action_best_id];
  q_res.q_value_best = actions_score[q_res.action_best_id];

  if ((learn != 0) || (force_learn != 0))
  {
    if (q_res_array_ptr != 0)
      q_res_array_ptr--;

    q_res_array[q_res_array_ptr] = q_res;
  }

  if ( ((q_res_array_ptr == 0) && (learn != 0)) || (force_learn != 0) )
  {
  //  for (j = 0; j < 10; j++)

    for (i = q_res_array_ptr; i < q_res_array.size(); i++)
    {
        float value = q_res_array[i].reward;

        if (i != 0)
           value+= q_init.gamma*get_highest_q(q_res_array[i-1].state, actions);

        if (value > 1.0)
          value = 1.0;

        if (value < -1.0)
          value = -1.0;

        q_res_array[i].q_value_best = value;

        switch (q_init.function_type)
        {
            case 0: q_func->learn(q_res_array[i].state, q_res_array[i].action, q_res_array[i].q_value_best);
                    break;

            case 1: q_func_nn_mcp->learn(q_res_array[i].state, q_res_array[i].action, q_res_array[i].q_value_best, q_res_array[i].action_best_id);
                    break;

            case 2: q_func_nn_tn->learn(q_res_array[i].state, q_res_array[i].action, q_res_array[i].q_value_best, q_res_array[i].action_best_id);
                    break;

            case 3: q_func_nn_knn->learn(q_res_array[i].state, q_res_array[i].action, q_res_array[i].q_value_best);
                    break;
        }
    }

    q_res_array_ptr = q_res_array.size() - 1;
  }
}

void CQlearning::reset()
{
  /*
  q_res.action_id = 0;
  q_res.action_best_id = 0;
  q_res.q_value = 0.0;
  q_res.q_value_best = 0.0;

  q_res_array[q_res_array_ptr] = q_res;
  if (q_res_array_ptr != 0)
    q_res_array_ptr--;
  */
}

struct sQlearningRes CQlearning::get()
{
  return q_res;
}

void CQlearning::save()
{
  if (q_func_nn_knn != NULL)
    q_func_nn_knn->save(NULL);
}

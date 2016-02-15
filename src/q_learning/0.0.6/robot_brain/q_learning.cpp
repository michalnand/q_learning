#include "q_learning.h"

CQlearning::CQlearning(struct sQlearningInit ql_init_struct)
{
  this->q_init = ql_init_struct;

  u32 i;
  for (i = 0; i < q_init.state_vector_size; i++)
    q_res.state.push_back(0.0);

  q_res.reward = 0.0;

  for (i = 0; i < q_init.action_vector_size; i++)
  {
    q_res.action.push_back(0.0);
    q_res.action_best.push_back(0.0);
  }


  q_res.best_action = 0;
  q_res.selected_action = 0;
  q_res.best_action_q = 0.0;
  q_res.selected_action_q = 0.0;

  q_res_prev = q_res;

  q_func = new  CQFunc(
                        q_init.state_vector_size, q_init.action_vector_size,
                        q_init.density, q_init.density,
                        q_init.alpha
                      );

  q_func_nn_mcp = new  CQFuncNN(
                                q_init.state_vector_size, q_init.action_vector_size,
                                q_init.density, q_init.density,
                                q_init.alpha, NN_LAYER_NEURON_TYPE_TANH
                                );

  q_func_nn_tn = new  CQFuncNN(
                                q_init.state_vector_size, q_init.action_vector_size,
                                q_init.density, q_init.density,
                                q_init.alpha, NN_LAYER_NEURON_TYPE_INTERSYNAPTICS
                              );

  q_func_nn_knn = new CQFuncKNN(
                              q_init.state_vector_size, q_init.action_vector_size,
                              q_init.density, q_init.density,
                              q_init.alpha, NN_LAYER_NEURON_TYPE_INTERSYNAPTICS
                            );

  for (i = 0; i < q_init.actions_count; i++)
  {
    actions_score.push_back(0.0);
    action_score_normalized.push_back(0.0);
  }

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
  delete q_func;
  delete q_func_nn_mcp;
  delete q_func_nn_tn;
  delete q_func_nn_knn;
}



void CQlearning::process(std::vector<float> state, std::vector<std::vector<float>> actions, float reward, u32 learn)
{
  q_res_prev = q_res;

  q_res.state = state;
  q_res.reward = reward;

  u32 j;

  switch (q_init.function_type)
  {
    case 0:
            for (j = 0; j < q_init.actions_count; j++)
              actions_score[j] = q_func->get(state, actions[j]);
            break;

    case 1:
            for (j = 0; j < q_init.actions_count; j++)
              actions_score[j] = q_func_nn_mcp->get(state, actions[j]);
            break;

    case 2:
            for (j = 0; j < q_init.actions_count; j++)
              actions_score[j] = q_func_nn_tn->get(state, actions[j]);
            break;

    case 3:
            for (j = 0; j < q_init.actions_count; j++)
              actions_score[j] = q_func_nn_knn->get(state, actions[j]);
            break;
  }

  //find best action
  q_res.best_action = 0;

  for (j = 0; j < q_init.actions_count; j++)
    if (actions_score[j] > actions_score[q_res.best_action])
    {
        q_res.best_action = j;
    }

  q_res.best_action_q = actions_score[q_res.best_action];
  q_res.action_best = actions[q_res.best_action];


  //normalise actions results into <0, 1>
  float min = 1000000000.0;
  float max = -min;

  for (j = 0; j < q_init.actions_count; j++)
  {
    if (actions_score[j] < min)
      min = actions_score[j];

    if (actions_score[j] > max)
      max = actions_score[j];
  }



  if (min == max)
  {
    for (j = 0; j < q_init.actions_count; j++)
      action_score_normalized[j] = 0.0;
  }
  else
  {
    float k = (1.0 - 0.0)/(max - min);
    float q = 1.0 - k*max;
    float sum = 0.0;

    for (j = 0; j < q_init.actions_count; j++)
    {
      action_score_normalized[j] = k*actions_score[j] + q;
      sum+= action_score_normalized[j];
    }

    for (j = 0; j < q_init.actions_count; j++)
      action_score_normalized[j]/= sum;
  }


  q_res.action_score_normalized = action_score_normalized;

  //select action, using exponential distribution, with k parameter
  q_res.selected_action = rand()%q_init.actions_count;
  q_res.best_action_q = actions_score[q_res.best_action];

  q_res.action = actions[q_res.selected_action];

  if (learn != 0)
  {
    float value = tanh(q_init.gamma*q_res_prev.reward + q_init.gamma*q_res.best_action_q);
    // q_func->learn(q_res_prev.state, q_res_prev.action, value);

    switch (q_init.function_type)
    {
      case 0: q_func->learn(q_res_prev.state, q_res_prev.action, value);
              break;

      case 1: q_func_nn_mcp->learn(q_res_prev.state, q_res_prev.action, value);
              break;

      case 2: q_func_nn_tn->learn(q_res_prev.state, q_res_prev.action, value);
              break;

      case 3: q_func_nn_knn->learn(q_res_prev.state, q_res_prev.action, value);
              break;
    }
  }
}

void CQlearning::reset()
{
  q_res_prev = q_res;

  //q_res.reward = 0.0;
  q_res.best_action = 0;
  q_res.selected_action = 0;
  q_res.best_action_q = 0.0;
  q_res.selected_action_q = 0.0;


}

struct sQlearningRes CQlearning::get()
{
  return q_res;
}

float CQlearning::get_max_q(std::vector<float> state, std::vector<std::vector<float>> actions)
{
  u32 j;
  float tmp, max_q = -1000000000.0;

  for (j = 0; j < q_init.actions_count; j++)
  {
    tmp = 0.0;
    switch (q_init.function_type)
    {
      case 0:
              tmp = q_func->get(state, actions[j]);
              break;

      case 1:
              tmp = q_func_nn_mcp->get(state, actions[j]);
              break;

      case 2:
              tmp = q_func_nn_tn->get(state, actions[j]);
              break;

      case 3:
              tmp = q_func_nn_knn->get(state, actions[j]);
              break;
    }

    if (tmp > max_q)
      max_q = tmp;
  }

  return max_q;
}

u32 CQlearning::get_max_q_action_id(std::vector<float> state, std::vector<std::vector<float>> actions)
{
  u32 j, max_id = 0;
  float tmp, max_q = -1000000000.0;

  for (j = 0; j < q_init.actions_count; j++)
  {
    tmp = 0.0;
    switch (q_init.function_type)
    {
      case 0:
              tmp = q_func->get(state, actions[j]);
              break;

      case 1:
              tmp = q_func_nn_mcp->get(state, actions[j]);
              break;

      case 2:
              tmp = q_func_nn_tn->get(state, actions[j]);
              break;

      case 3:
              tmp = q_func_nn_knn->get(state, actions[j]);
              break;
    }

    if (tmp > max_q)
    {
      max_q = tmp;
      max_id = j;
    }
  }

  return max_id;
}

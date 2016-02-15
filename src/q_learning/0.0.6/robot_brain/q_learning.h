#ifndef _Q_LEARNING_H_
#define _Q_LEARNING_H_

#include "../common.h"
#include "q_func/q_func.h"
#include "q_func/q_func_nn.h"
#include "q_func/q_func_knn.h"



struct sQlearningInit
{
  u32 state_vector_size;
  u32 action_vector_size;

  u32 actions_count;

  u32 function_type;

  float gamma, alpha, k;
  float density;
};


struct sQlearningRes
{
float reward;
  std::vector<float> state, action, action_best;
  u32 selected_action, best_action;
  float best_action_q, selected_action_q;

  std::vector<float> action_score_normalized;
};

class CQlearning
{
  private:
    struct sQlearningInit q_init;
    struct sQlearningRes q_res, q_res_prev;

    std::vector<float> actions_score, action_score_normalized;

    class CQFunc *q_func;
    class CQFuncNN *q_func_nn_mcp;
    class CQFuncNN *q_func_nn_tn;
    class CQFuncKNN *q_func_nn_knn;

  public:
    CQlearning(struct sQlearningInit ql_init_struct);
    ~CQlearning();

    void process(std::vector<float> state, std::vector<std::vector<float>> actions, float reward, u32 learn);
    void reset();

    struct sQlearningRes get();

    float get_max_q(std::vector<float> state, std::vector<std::vector<float>> actions);
    u32 get_max_q_action_id(std::vector<float> state, std::vector<std::vector<float>> actions);
};


#endif

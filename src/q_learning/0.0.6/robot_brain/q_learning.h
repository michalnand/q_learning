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
  float               reward;
  std::vector<float>  state;

  std::vector<float>  action,     action_best;
  u32                 action_id,  action_best_id;
  float               q_value,    q_value_best;
};


class CQlearning
{
  private:
    struct sQlearningInit q_init;
    struct sQlearningRes q_res;

    u32 q_res_array_ptr;
    std::vector<struct sQlearningRes> q_res_array;

    std::vector<float> actions_score;

    class CQFunc    *q_func;
    class CQFuncNN  *q_func_nn_mcp;
    class CQFuncNN  *q_func_nn_tn;
    class CQFuncKNN *q_func_nn_knn;

  public:
    CQlearning(struct sQlearningInit ql_init_struct);
    ~CQlearning();

    float get_highest_q(std::vector<float> state, std::vector<std::vector<float>> actions);

    void process(std::vector<float> state, std::vector<std::vector<float>> actions, float reward = 0.0, u32 learn = 0, u32 force_learn = 0);
    void reset();

    struct sQlearningRes get();

    void save();

};


#endif

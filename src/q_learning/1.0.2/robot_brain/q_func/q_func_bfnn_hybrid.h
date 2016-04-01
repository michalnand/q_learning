#ifndef _Q_FUNC_BFNN_HYBRID_H_
#define _Q_FUNC_BFNN_HYBRID_H_

#include "neural_network/basis_functions_hybrid.h"
#include "../../common.h"

class CQFuncBFNNHybrid
{
  private:
    std::vector<std::vector<float>> actions;
    std::vector<class CBasisFunctionsHybrid*> bf_nn;

    float learning_rate;

  public:
    CQFuncBFNNHybrid(u32 state_size, std::vector<std::vector<float>> actions);
    ~CQFuncBFNNHybrid();

    float get(std::vector<float> state, std::vector<float> action);
    void learn(std::vector<float> state, std::vector<float> action, float required_value);

    i32 save(char *file_name);
    i32 load(char *file_name);

    u32 get_action_idx(std::vector<float> action);
};


#endif

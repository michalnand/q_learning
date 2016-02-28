#ifndef _Q_FUNC_BFNN_H_
#define _Q_FUNC_BFNN_H_

#include "../../common.h"
#include "neural_network/nn.h"
#include "neural_network/kohonen_layer.h"
#include "neural_network/basis_functions.h"

#define Q_FUNC_BFNN_MAGIC    (u32)0xABCD0002


#define BFNN_LINEAR           0
#define BFNN_TANH             1
#define BFNN_INTERSYNAPTICS   2


class CQFuncBFNN
{
    private:
        std::vector<std::vector<float>> actions;

        std::vector<class CArray*> q_tables;
        std::vector<class CBasisFunctions*> bf_nn;
        std::vector<class CNN*> nn;

        float alpha;

        std::vector<float> bf_nn_input;
        std::vector<float> q_idx_f;

        u32 network_type;

    public:
        CQFuncBFNN( u32 state_size, u32 action_size,
                float density, float action_density,
                float alpha, u32 network_type,
                std::vector<std::vector<float>> actions);
        ~CQFuncBFNN();

        void learn_start();
        void learn_finish();

        float get(std::vector<float> state, std::vector<float> action);
        void learn(std::vector<float> state, std::vector<float> action, float required_value);

        i32 save(char *file_name);
        i32 load(char *file_name);

        void merge(class CQFuncBFNN *q_func);
        u32 get_action_idx(std::vector<float> action);
};

#endif

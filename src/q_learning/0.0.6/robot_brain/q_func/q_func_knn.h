#ifndef _Q_FUNC_NNK_H_
#define _Q_FUNC_NNK_H_

#include "../../common.h"
#include "neural_network/nn.h"
#include "neural_network/kohonen_layer.h"
#include "neural_network/basis_functions.h"

#define Q_FUNC_KNN_MAGIC    (u32)0xABCD0002


class CQFuncKNN
{
    private:
        std::vector<std::vector<float>> actions;

        std::vector<class CArray*> q_tables;
        std::vector<class CNN*> nn;

        std::vector<class CBasisFunctions*> bf_nn;

        float alpha;

        std::vector<float> nn_input, bf_nn_input;
        std::vector<float> q_idx_f;

    public:
        CQFuncKNN( u32 state_size, u32 action_size,
                float density, float action_density,
                float alpha, u32 neuron_type,
                std::vector<std::vector<float>> actions);
        ~CQFuncKNN();

        void learn_start();
        void learn_finish();

        float get(std::vector<float> state, std::vector<float> action);
        void learn(std::vector<float> state, std::vector<float> action, float required_value);

        i32 save(char *file_name);
        i32 load(char *file_name);

        void merge(class CQFuncKNN *q_func);
        u32 get_action_idx(std::vector<float> action);
};

#endif

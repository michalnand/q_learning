#ifndef _Q_FUNC_NN_H_
#define _Q_FUNC_NN_H_

#include "../../common.h"
#include "neural_network/nn.h"
#include "neural_network/kohonen_layer.h"

class CQFuncNN
{
    private:
        std::vector<class CNN*> nn;

        float alpha;

        std::vector<float> nn_input;

    public:
        CQFuncNN( u32 state_size, u32 action_size,
                float density, float action_density,
                float alpha = 0.9, u32 neuron_type = NN_LAYER_NEURON_TYPE_TANH, u32 actions_count = 1);
        ~CQFuncNN();


        float get(std::vector<float> state, std::vector<float> action, u32 action_id);
        void learn(std::vector<float> state, std::vector<float> action, float required_value, u32 action_id);

        i32 save(char *file_name);
        i32 load(char *file_name);
        void merge(class CQFuncNN *q_func);
};

#endif

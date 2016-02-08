#ifndef _Q_FUNC_ENN_H_
#define _Q_FUNC_ENN_H_

#include "../common.h"
#include "neural_network_echo_state.h"


class CQFuncENN
{
    private:
        class CEchoStateNeuralNetwork *nn;

        float alpha;

        std::vector<float> nn_input;

    public:
        CQFuncENN( u32 state_size, u32 action_size,
                float density, float action_density,
                float alpha = 0.9);
        ~CQFuncENN();

        float get(std::vector<float> state, std::vector<float> action);
        void learn(std::vector<float> state, std::vector<float> action, float required_value);

        i32 save(char *file_name);
        i32 load(char *file_name);
        void merge(class CQFuncENN *q_func);

};

#endif

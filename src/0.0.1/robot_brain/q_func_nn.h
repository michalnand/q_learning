#ifndef _Q_FUNC_NN_H_
#define _Q_FUNC_NN_H_

#include "../common.h"
#include "neural_network.h"


class CQFuncNN
{
    private:
        class CNeuralNetwork *nn;

        float alpha;

        std::vector<float> nn_input;

    public:
        CQFuncNN( u32 state_size, u32 action_size,
                float density, float action_density,
                float alpha = 0.9);
        ~CQFuncNN();

        float get(std::vector<float> state, std::vector<float> action);
        void learn(std::vector<float> state, std::vector<float> action, float required_value);
};

#endif

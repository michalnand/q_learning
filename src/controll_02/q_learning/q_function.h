#ifndef _Q_FUNCTION_H_
#define _Q_FUNCTION_H_

#include "../common.h"
#include "kohonen_neural_network.h"

class CQfunction
{
    private:
        u32 state_vector_size, action_vector_size;
        float alpha;

        std::vector<std::vector<float>> q;

        u32 action_idx;
        u32 state_idx;
 
        class CKohonenNeuralNetwork *state_associative_array, *action_associative_array;

    public:
        CQfunction(u32 state_vector_size, u32 action_vector_size, float alpha);
        ~CQfunction();

        float get(std::vector<float> state, std::vector<float> action);

        void learn(std::vector<float> state, std::vector<float> action, float value);
};

#endif

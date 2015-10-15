#ifndef _Q_LEARNING_NEW_H_
#define _Q_LEARNING_NEW_H_

#include "../common.h"

#include "q_function.h"

class CQLearningNew
{
    private:
        class CQfunction *q_function;

        u32 state_vector_size;
        u32 action_vector_size;
        u32 actions_per_state;

        float gamma, alpha;
        float reward, reward_prev;

        std::vector<float> state, state_prev;
        std::vector<float> action, action_prev;

        std::vector<std::vector<float>> actions;

    public:
        CQLearningNew(u32 state_vector_size, u32 action_vector_size, u32 actions_per_state,
                    float gamma, float alpha
                    );

        ~CQLearningNew();
        void reset();

        void process(std::vector<float> state, float reward, float k);

        std::vector<float> get_action();
        std::vector<float> select_action(float k);
        std::vector<float> select_best_action();


        float get_max_q(std::vector<float> state);

        void print();



};

#endif

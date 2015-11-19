#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "../common.h"
#include "../q_learning/q_learning.h"
#include "../q_learning/q_learning_new.h"

#include "pid.h"

class CController
{
    private:
        class CQLearningNew *q_learning;
        class CAction 		*actions;

        float output;
        float dt;
        float reward, reward_sum;

        float q_max, q_max_id;

        std::vector<float> state;

        struct sPID pid;

    public:

        CController(float dt);
        ~CController();

        void reset();
        void process(float required_value, float plant_output, bool explore);
        float get();

        float get_reward();


        float get_q_max();
        u32 get_q_id_max();

        float get_reward_sum();

        std::vector<float> get_state();

        void print();


};

#endif

#include "controller.h"




CController::CController(float dt)
{
    this->dt  = dt;

    float state_density = 1.0/100.0;
    u32 state_dimensions = 3;
    u32 actions_per_state = 8;

    u32 outputs_count = 1;

    float gamma = 0.9;
    float alpha = 0.98;


    u32 j, i;

    for (j = 0; j < state_dimensions; j++)
        state.push_back(0.0);


    q_learning = new CQLearningNew(state_dimensions, outputs_count, actions_per_state,
                                   gamma, alpha);

    reset();
}

CController::~CController()
{
    delete actions;
    delete q_learning;
}


void CController::reset()
{
    output = 0.0;
    reward = 0.0;
    reward_sum = 0.0;


    u32 j;
    for (j = 0; j < state.size(); j++)
        state[j] = 0.0;

    q_learning->reset();
}

void CController::process(float required_value, float plant_output, bool explore)
{
    float error = required_value - plant_output;

    float f = 0.7;
    reward = f*reward + (1.0 - f)*pow(4.0, 1.0 - abs_(tanh(error)));

    reward_sum+= reward;



    state[2] = state[1];
    state[1] = state[0];
    //state[0] = sgn(error)*pow(2.0, tanh( error / 2.0));
    state[0] = tanh(error);

    q_learning->process(state, reward, 1.1);

    /*
    float res = 0.0;

    if (abs_(state[0]) < 0.1)
        res = 0.0;
    else
    if (state[0] > 0)
        res = 1.0;
    else
        res = -1.0;

    */

    q_max = q_learning->get_max_q(state);

    std::vector<float> action;

    if (explore == true)
        action = q_learning->get_action();
    else
        action = q_learning->select_best_action();


    float limit = 2.0;
    output = action[0];

    if (output > limit)
        output = limit;

    if (output < -limit)
        output = -limit;
}

float CController::get()
{
    return output;
}

float CController::get_reward()
{
    return reward;
}

float CController::get_q_max()
{
    return q_max;
}

u32 CController::get_q_id_max()
{
    return q_max_id;
}

float CController::get_reward_sum()
{
    return reward_sum;
}

std::vector<float> CController::get_state()
{
    return state;
}


void CController::print()
{
    q_learning->print();
}

#include "controller.h"

CController::CController(float dt)
{
    this->dt  = dt;

    float state_density = 1.0/4.0;
    u32 state_dimensions = 3;
    u32 actions_per_state = 6;

    u32 outputs_count = 3;

    float gamma = 0.9;
    float alpha = 0.98;

    u32 j, i;

    for (j = 0; j < state_dimensions; j++)
        state.push_back(0.0);


    q_learning = new CQLearningNew(state_dimensions, outputs_count, actions_per_state,
                                   gamma, alpha);

  //  pid_init(&pid, 0.3, 0.03, 0.0, 2.0);
    pid_init(&pid, 0.0, 0.0, 0.0, 2.0);
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

    pid_init(&pid, 0.0, 0.0, 0.0, 2.0);

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
    state[0] = sgn(error)*log(tanh(abs_(error)) + 0.01);
    //state[0] = tanh(error);

    q_learning->process(state, reward, 1.1);

    q_max = q_learning->get_max_q(state);

    std::vector<float> action;

    if (explore == true)
        action = q_learning->get_action();
    else
        action = q_learning->select_best_action();


    float k = 0.01;
    float dkp = action[0]*k;
    float dki = action[1]*k;
    float dkd = action[2]*k;
    pid_set_dif(&pid, dkp, dki, dkd, 2.0);


/*
    float kp = action[0];
    float ki = action[1];
    float kd = action[2];

    pid_set(&pid, kp, ki, kd, 2.0);
    output = pid_process(&pid, error);
*/

    output = pid_process(&pid, error);
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

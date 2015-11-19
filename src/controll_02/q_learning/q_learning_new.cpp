#include "q_learning_new.h"


CQLearningNew::CQLearningNew(u32 state_vector_size, u32 action_vector_size, u32 actions_per_state,
            float gamma, float alpha
            )
{
    this->state_vector_size = state_vector_size;
    this->action_vector_size = action_vector_size;
    this->actions_per_state = actions_per_state;

    this->gamma = gamma;
    this->alpha = alpha;

    q_function = new CQfunction(state_vector_size, action_vector_size, alpha);


    u32 j, i;
    for (j = 0; j < actions_per_state; j++)
    {
        std::vector<float> tmp;

        for (i = 0; i < action_vector_size; i++)
        {
          tmp.push_back(0.0);
          // tmp[i] = ( 2.2*((j%actions_per_state) - (actions_per_state/2.0))/actions_per_state );
        }

        if ((j%2) == 0)
          tmp[j/2] = 1.0;
        else
          tmp[j/2] = -1.0;

        for (i = 0; i < action_vector_size; i++)
          printf("%6.3f ", tmp[i]);

        printf("\n");

        actions.push_back(tmp);
    }

    for (i = 0; i < action_vector_size; i++)
    {
        action.push_back(0.0);
        action_prev.push_back(0.0);
    }

    for (i = 0; i < state_vector_size; i++)
    {
        state.push_back(0.0);
        state_prev.push_back(0.0);
    }

    reward = 0.0;
    reward_prev = 0.0;


}

CQLearningNew::~CQLearningNew()
{
    delete q_function;
}

void CQLearningNew::reset()
{
    u32 i;

    for (i = 0; i < action_vector_size; i++)
    {
        action[i] = 0.0;
        action_prev[i] = 0.0;
    }

    for (i = 0; i < state_vector_size; i++)
    {
        state[i] = 0.0;
        state_prev[i] = 0.0;
    }

    reward = 0.0;
    reward_prev = 0.0;

}

void CQLearningNew::process(std::vector<float> state, float reward, float k)
{
    this->state_prev = this->state;
    this->action_prev = this->action;
    this->reward_prev = this->reward;

    this->state = state;
    this->action = select_action(k);
    this->reward = reward;

    float tmp = reward_prev + gamma*get_max_q(state);

    q_function->learn(state_prev, action_prev, tmp);
}

std::vector<float> CQLearningNew::get_action()
{
    return action;
}

std::vector<float> CQLearningNew::select_action(float k)
{
    std::vector<float> p_tmp;

    float sum = 0.0;

    u32 j;
    for (j = 0; j < actions_per_state; j++)
    {
        float tmp = q_function->get(state, actions[j]);
        p_tmp.push_back( pow(k, tmp ) );
        sum+= p_tmp[j];
    }

    float p = abs_(rnd_());
    float sum_tmp = 0.0;
    for (j = 0; j < actions_per_state; j++)
    {
        sum_tmp+= p_tmp[j]/sum;

        if (p < sum_tmp)
            break;
    }

    if (j >= actions_per_state)
        j = 0; //actions_per_state-1;

    // j = rand()%actions_per_state;

    return actions[j];
}

std::vector<float> CQLearningNew::select_best_action()
{
    u32 best_idx = 0;

    float best_q = -1000000.0;

    u32 j;
    for (j = 0; j < actions_per_state; j++)
    {
        float tmp = q_function->get(state, actions[j]);

        if (tmp > best_q)
        {
            best_q = tmp;
            best_idx = j;
        }
    }

    return actions[best_idx];
}


float CQLearningNew::get_max_q(std::vector<float> state)
{
    u32 j;
    float max_q = -1.0;

    for (j = 0; j < actions_per_state; j++)
    {
        float tmp = q_function->get(state, actions[j]);

        if (tmp > max_q)
            max_q = tmp;
    }

    return max_q;
}

void CQLearningNew::print()
{
    std::vector<float> state;
    float dx = 1.0/10.0;

    u32 j;

    float s;

    for (j = 0; j < state_vector_size; j++)
        state.push_back(0.0);

    for (s = -1.0; s < 1.0; s+= dx)
    {
        state[0] = s;
        printf("%6.3f : ", state[0]);

        for (j = 0; j < actions_per_state; j++)
        {
            float q =  q_function->get(state, actions[j]);
            printf("%6.3f ", q);
        }

        printf("\n");
    }
    printf("\n");
    printf("\n");
}

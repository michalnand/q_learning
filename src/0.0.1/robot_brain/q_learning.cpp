#include "q_learning.h"

void q_res_init(struct sQRes *q_res, u32 state_dimensions, u32 action_size)
{
	u32 i;

	q_res->q = 0.0;
	q_res->q_max = 0.0;
	q_res->action_id = 0;

	q_res->action.fitness = 0.0;
	q_res->action.usability = 0.0;

	q_res->reward = 0.0;

	for (i = 0; i < state_dimensions; i++)
		q_res->state.push_back(0.0);

	for (i = 0; i < action_size; i++)
		q_res->action.action.push_back(0.0);
}


CQLearning::CQLearning(
						float states_density,
						u32 actions_per_state,
						u32 state_dimensions,
						float gamma, float alpha,
						class CAction *actions
	 					)
{
	this->actions = actions;


 	this->gamma = gamma;
	this->alpha = alpha;

	this->states_density = states_density;

	float actions_density = 1.0;

	#ifdef Q_FUNC_NN
	q_func = new CQFuncNN(state_dimensions, actions->get_action_size(), states_density, actions_density, alpha);
	#else
	#ifdef Q_FUNC_KNN
	q_func = new CQFuncKNN(state_dimensions, actions->get_action_size(), states_density, actions_density, alpha);
	#else
	q_func = new CQFunc(state_dimensions, actions->get_action_size(), states_density, actions_density, alpha);
	#endif
	#endif

	q_res_init(&q_res, state_dimensions, actions->get_action_size());
	q_res_init(&q_res_prev, state_dimensions, actions->get_action_size());
}

CQLearning::~CQLearning()
{
	delete q_func;
}



void CQLearning::process(std::vector<float> state, float reward, float k, float explore_prob)
{
	//find action using current state and fitness as probability
	q_res.state = state;

	q_res.action_id = select_action(q_res.state, k, explore_prob);
	q_res.action = actions->get(0, q_res.action_id);

	q_res.q = q_func->get(q_res.state, q_res.action.action);
	q_res.reward = reward;
	q_res.q_max = get_max_q(q_res.state);

	float tmp = q_res_prev.reward + gamma*q_res.q_max;
	q_func->learn(q_res_prev.state, q_res_prev.action.action, tmp);

	q_res_prev = q_res;
}

u32 CQLearning::get_output_id()
{
	return q_res.action_id;
}


float CQLearning::get_max_q(std::vector<float> state)
{
	u32 i;
	float max_q = -1.0;
	for (i = 0; i < actions->get_actions_per_state(); i++)
	{
		float tmp = q_func->get(state, actions->get(0, i).action);
		if (tmp > max_q)
			max_q = tmp;
	}

	return max_q;
}

float CQLearning::get_min_q(std::vector<float> state)
{
	u32 i;
	float min_q = 1.0;
	for (i = 0; i < actions->get_actions_per_state(); i++)
	{
		float tmp = q_func->get(state, actions->get(0, i).action);
		if (tmp < min_q)
			min_q = tmp;
	}

	return min_q;
}

u32 CQLearning::get_max_q_id(std::vector<float> state)
{
	u32 i, max_i = 1.0;
	float max_q = -1.0;
	for (i = 0; i < actions->get_actions_per_state(); i++)
	{
		float tmp = q_func->get(state, actions->get(0, i).action);
		if (tmp > max_q)
		{
			max_q = tmp;
			max_i = i;
		}
	}

	return max_i;
}

u32 CQLearning::select_action(std::vector<float> state, float k, float explore_prob)
{
	u32 i;

	float sum = 0.0;
	float sum_tmp = 0.0;
	float p = abs_(rnd_());

	std::vector<float> q_res_values;

	for (i = 0; i < actions->get_actions_per_state(); i++)
	{
		struct sAction action = actions->get(0, i);
		q_res_values.push_back(q_func->get(state, action.action));
	}

	u32 non_visited_action_id = 0;
	bool non_visited_action_found = false;

	for (i = 0; i < q_res_values.size(); i++)
	{
		sum+= pow(k, q_res_values[i]);
		if (q_res_values[i] == -1.0)
		{
			non_visited_action_id = i;
			non_visited_action_found = true;
		}
	}

	p = abs_(rnd_());

	if ( (non_visited_action_found == true) && (p < explore_prob ) )
	{
		return non_visited_action_id;
	}
	else
	for (i = 0; i < q_res_values.size(); i++)
	{
		sum_tmp+= pow(k, q_res_values[i]);

		if (p < (sum_tmp/sum))
			return i;
	}

	return 0;
}

#ifdef Q_FUNC_NN
class CQFuncNN* CQLearning::get_func()
{
	return q_func;
}
#else
#ifdef Q_FUNC_KNN
class CQFuncKNN* CQLearning::get_func()
{
	return q_func;
}
#else
class CQFunc* CQLearning::get_func()
{
	return q_func;
}
#endif
#endif

void CQLearning::merge(CQLearning *q_learning)
{

}

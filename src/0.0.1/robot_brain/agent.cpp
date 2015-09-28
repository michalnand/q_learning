#include "agent.h"

void s_agent_init(
					struct sAgent *agent,

					u32 id, u32 type,
					u32 outputs_count, u32 inputs_count,
		 			u32 actions_per_state,
					float state_density
				)

{
	agent->id = id;
	agent->type = type;
	agent->outputs_count = outputs_count;
	agent->inputs_count = inputs_count;
	agent->actions_per_state = actions_per_state;

	agent->state_density = state_density;

	u32 i;

	for (i = 0; i < inputs_count; i++)
		agent->state.push_back(0.0);


	agent->output_action.fitness = 0.0;
	for (i = 0; i < outputs_count; i++)
		agent->output_action.action.push_back(0.0);
}

CAgent::CAgent(struct sAgent agent_init, class CAgent *collective_agent)
{
	u32 i, j;

	/*
	float alpha = 0.8;
	float gamma = 0.7;
	*/

	float alpha = 0.98;
	float gamma = 0.7;

	std::vector<std::vector<float>> action_init;

	u32 state_dimensions = agent_init.state.size();

	this->agent = agent_init;
	this->collective_agent = collective_agent;

	for (j = 0; j < agent_init.actions_per_state; j++)
	{
		std::vector<float> tmp;
		for (i = 0; i < agent_init.outputs_count; i++)
		{
			tmp.push_back(0.0);

			if ( i == (j/2) )
			{
				if ((j%2) == 0)
					tmp[i] = 1.0;
				else
					tmp[i] = -1.0;
			}
		}

		action_init.push_back(tmp);
	}

	actions = new CAction(	1,
							action_init.size(),
							action_init[0].size(),
							&action_init
						);

	q_learning = new CQLearning(
									agent_init.state_density,
									agent_init.actions_per_state,
									state_dimensions,
									gamma, alpha,
									actions
								);
}

CAgent::CAgent(char *file_name, class CAgent *collective_agent)
{
	//TODO load from file
}

void CAgent::save(char *file_name)
{
	//TODO save into file
}

CAgent::~CAgent()
{
	if (q_learning != NULL)
	{
		delete q_learning;
		q_learning = NULL;
	}

	if (actions != NULL)
	{
		delete actions;
		actions = NULL;
	}
}

void CAgent::process(struct sAgent *agent_)
{
	u32 i, action_id;
	std::vector<float> state_vect;

	agent = *agent_;

	for (i = 0; i < this->agent.inputs_count; i++)
		state_vect.push_back(agent.state[i]);

	float k = 2.0;
	float explore_probality = 0.0;

	switch (agent.type)
	{
		case AGENT_TYPE_EXPLORER : k = 1.1; explore_probality = 0.9; break;
		case AGENT_TYPE_COMMON : k = 2.0; explore_probality = 0.0; break;
		case AGENT_TYPE_GREEDY : k = 10.0; explore_probality = 0.0; break;
	}

	q_learning->process(state_vect, agent.reward, k, explore_probality);
	agent.reward = 0.0;

	agent.score+= 1.0;

	action_id = q_learning->get_output_id();

	//in all states are some action, so state is allways 0
	agent.output_action = actions->get(0, action_id);

	*agent_ = agent;
}


//agent output
struct sAction CAgent::get_action()
{
	return agent.output_action;
}

#ifdef Q_LEARNING_NN
//merging functions
CQLearningNN* CAgent::get_brain()
{
	return q_learning;
}
#else
//merging functions
CQLearning* CAgent::get_brain()
{
	return q_learning;
}
#endif

void CAgent::merge()
{
	if (collective_agent != NULL)
		q_learning->merge(collective_agent->get_brain());
}

void CAgent::print(std::vector<float> subspace)
{
	CLog log((char*)"q_map.txt", 3);

	u32 i;
	float y, x;
	std::vector<float> state;

	for (i = 0; i < agent.inputs_count; i++)
		state.push_back(0.0);

	for (i = 0; i < subspace.size(); i++)
		state[i] = subspace[i];

	printf("\n q max \n");
	for (y = -1.0; y < 1.0; y+= agent.state_density)
	{
		for (x = -1.0; x < 1.0; x+= agent.state_density)
		{
			state[subspace.size() + 1] = y;
			state[subspace.size() + 0] = x;

			float res = q_learning->get_max_q(state);

			printf("%2.3f ", res);

			//u32 state_idx = q_learning->get_state_idx();
			//printf("%3u ", state_idx);
			log.add(0, x);
			log.add(1, y);
			log.add(2, res);
		}

		printf("\n");
	}

	log.save();


	printf("\n\n");


	printf("\n q min \n");
	for (y = -1.0; y < 1.0; y+= agent.state_density)
	{
		for (x = -1.0; x < 1.0; x+= agent.state_density)
		{
			state[subspace.size() + 1] = y;
			state[subspace.size() + 0] = x;

			float res = q_learning->get_min_q(state);

			printf("%2.3f ", res);
		}

		printf("\n");
	}

	printf("\n\n");



	printf("\n q id \n");

	CLog log_action_id((char*)"q_action_id.txt", 3);

	for (y = -1.0; y < 1.0; y+= agent.state_density)
	{
		for (x = -1.0; x < 1.0; x+= agent.state_density)
		{
			state[subspace.size() + 1] = y;
			state[subspace.size() + 0] = x;

			u32 res = q_learning->get_max_q_id(state);

			printf("%3i ", res);

			log_action_id.add(0, x);
			log_action_id.add(1, y);
			log_action_id.add(2, res);
		}

		printf("\n");
	}

	printf("\n\n");

	log_action_id.save();


	printf("\nbest action matrix \n");

	CLog log_action((char*)"q_action.txt", 2 + actions->get_action_size());
	for (y = -1.0; y < 1.0; y+= agent.state_density)
	{
		for (x = -1.0; x < 1.0; x+= agent.state_density)
		{
			state[subspace.size() + 1] = y;
			state[subspace.size() + 0] = x;

			u32 action_id = q_learning->get_max_q_id(state);

			struct sAction action = actions->get(0, action_id);

			vect_print(action.action);

			log_action.add(0, x);
			log_action.add(1, y);
			for (i = 0; i < action.action.size(); i++)
				log_action.add(2 + i, action.action[i]);
		}

		printf("\n");
	}

	printf("\n\n");

	log_action.save();
}

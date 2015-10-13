#include "action.h"

CAction::CAction(u32 states_count, u32 actions_per_state, u32 action_width, std::vector<std::vector<float>> *action_init)
{
	u32 k, j, i;

	for (k = 0; k < states_count; k++)
	{
		//actions in one state
		std::vector<struct sAction> actions_tmp;

		//actions initialziation
		for (j = 0; j < actions_per_state; j++)
		{
			struct sAction action;

			for (i = 0; i < action_width; i++)
				action.action.push_back(0.0);

			if (action_init != NULL)
			{
				for (i = 0; i < action_width; i++)
					action.action[i] = (*action_init)[j][i];

			}
			else
			{
				for (i = 0; i < action_width; i++)
					action.action[i] = sgn(rnd_());
			}

			action.fitness = ACTION_FITNESS_MIN;
			action.usability = 0.0;
			actions_tmp.push_back(action);
		}

		actions.push_back(actions_tmp);
	}
}

CAction::~CAction()
{

}

struct sAction CAction::get(u32 state, u32 id)
{
	return actions[state][id];
}

void CAction::set(u32 state, u32 id, struct sAction action, float weight)
{
	u32 i;

	for (i = 0; i < actions[state][id].action.size(); i++)
	{
		float res = actions[state][id].action[i]*(1.0 - weight) + action.action[i]*weight;
		actions[state][id].action[i] = res;
	}
}

void CAction::set_fitness(u32 state, u32 id, float fitness)
{
	actions[state][id].fitness = fitness;
}

u32 CAction::get_states_count()
{
	return actions.size();
}

u32 CAction::get_actions_per_state()
{
	return actions[0].size();
}

u32 CAction::get_action_size()
{
	return actions[0][0].action.size();
}

void CAction::set_usability(u32 state, u32 id, float usability)
{
	actions[state][id].usability = usability;
}

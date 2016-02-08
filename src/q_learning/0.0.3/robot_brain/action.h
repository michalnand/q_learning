#ifndef _ACTION_H_
#define _ACTION_H_


#include "../common.h"

#define ACTION_FITNESS_MIN	(float)0.0

struct sAction
{
	float fitness, usability;
	std::vector<float> action;
};

class CAction
{
	private:
		std::vector<std::vector<struct sAction>> actions;

	public:

		CAction(u32 states_count, u32 actions_per_state, u32 action_width = 1, std::vector<std::vector<float>> *action_init = NULL);
		~CAction();

		struct sAction get(u32 state, u32 id);

		void set(u32 state, u32 id, struct sAction action, float weight);
		void set_fitness(u32 state, u32 id, float fitness);

		void set_usability(u32 state, u32 id, float usability);

		u32 get_states_count();
		u32 get_actions_per_state();
        u32 get_action_size();

	private:
		float rnd();
};


#endif

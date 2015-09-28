#include "environment.h"

/*
float vect_dist(std::vector<float> *va, std::vector<float> *vb)
{
	float res = 0.0;
	u32 j;

	for (j = 0; j < va->size(); j++)
	{
		float tmp = (*va)[j] - (*vb)[j];
		res+= tmp*tmp;
	}

	return sqrt(res);
}
*/



CEnvironment::CEnvironment(u32 agents_count)
{
 	u32 j;

 	u32 outputs_count = 1;
 	u32 agent_inputs_count = 2;
 	u32 actions_per_state = 2;

 	state_density = 1.0/8.0;

 	u32 id = 0;
 	u32 type = AGENT_TYPE_NULL;

 	s_agent_init(
				 	&agent_init,
				 	id, type,
					outputs_count, agent_inputs_count,
					actions_per_state,
					state_density
				);


 	//this robot is for sharing solution
 	collective_agent = NULL;
 	//collective_agent = new CAgent(agent_init, NULL);

 	for (j = 0; j < agents_count; j++)
 	{ 
 		id++;
 		agent_init.id = id;

        agent_init.type = AGENT_TYPE_EXPLORER;
        //agent_init.type = AGENT_TYPE_COMMON;
        //agent_init.type = AGENT_TYPE_GREEDY;

 		class CAgent *agent;

 		respawn(&agent_init);

 		agent = new CAgent(agent_init, collective_agent);
 		agents.push_back(agent);

 		s_agents.push_back(agent_init);
 	}

 	for (j = 0; j < agents.size(); j++)
		agents[j]->merge();

	target_position.push_back(0.0);
 	printf("init done\n");
}

CEnvironment::~CEnvironment()
{
	u32 j;
	for (j = 0; j < agents.size(); j++)
		if (agents[j] != NULL)
		{
			delete agents[j];
			agents[j] = NULL;
		}

	if (collective_agent != NULL)
	{
		delete collective_agent;
		collective_agent = NULL;
	}

 	printf("uninit done\n");
}

void CEnvironment::process()
{
	u32 j, i;



	for (j = 0; j < agents.size(); j++)
	{
		float target_min_dist = s_agents[j].state_density;

		float target_dist  = abs_(target_position[0] - s_agents[j].state[0]);
		float reward = 0.0;

		if (target_dist < target_min_dist)
			reward = 1.0;

        //if (abs_(s_agents[j].state[0] - 0.5) < target_min_dist)
        //    reward+= 0.5;

		s_agents[j].reward = reward;

		agents[j]->process(&s_agents[j]);

		// non zero reward -> some information found,
		// add this into collective brain
		if (reward != 0.0)
		{
			agents[j]->merge();

			//send this information to all robots
			for (i = 0; i < agents.size(); i++)
				agents[i]->merge();
		}

		if (target_dist < target_min_dist)
		{
			printf("robot %u on target, score %f\n", j, s_agents[j].score);

			s_agents[j].reward = 0.0;

			respawn(&s_agents[j]);

			target_position[0] = rnd_();
		}
		else
		{
			s_agents[j].state[0] = s_agents[j].state[0] +
									s_agents[j].output_action.action[0]*s_agents[j].state_density;

			s_agents[j].state[1] = target_position[0];

			s_agents[j].state[0] = saturate(s_agents[j].state[0], -1.0, 1.0);
			s_agents[j].state[1] = saturate(s_agents[j].state[1], -1.0, 1.0);
		}
	}
}

void CEnvironment::print(std::vector<float> subspace)
{
	u32 i;
	//send this information to all robots
	for (i = 0; i < agents.size(); i++)
		agents[i]->merge();

 	//collective_agent->print(subspace);

 	agents[0]->print(subspace);
}

void CEnvironment::respawn(struct sAgent *agent)
{
	u32 i;
	for (i = 0; i < agent->inputs_count; i++)
		agent->state[i] = 0.0;

	agent->state[0] = rnd_();	//agent to random place

	agent->score = 0.0;
}

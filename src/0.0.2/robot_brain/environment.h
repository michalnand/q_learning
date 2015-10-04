#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "agent.h"
#include "../common/map.h"

class CEnvironment
{
	private:

		struct sAgent agent_init;

		std::vector<class CAgent *> agents;
		std::vector<struct sAgent> s_agents;


		class CAgent *collective_agent;

		float state_density;

		std::vector<float> start_position, target_position;


		class CMap *map;

		u32 loops;
		
	public:
		CEnvironment(u32 agents_count);
		~CEnvironment();


		void process();
		void print(std::vector<float> subspace);

	private:
		void respawn(struct sAgent *agent);
};

#endif

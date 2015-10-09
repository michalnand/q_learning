#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "agent.h"
#include "../common/map.h"


#define VISUALISATION_SCREEN_WIDTH 		800
#define VISUALISATION_SCREEN_HEIGHT 	600
#define VISUALISATION_ROBOT_SIZE		55.0

class CEnvironment
{
	private:

		struct sAgent agent_init;

		std::vector<class CAgent *> agents;
		std::vector<struct sAgent> s_agents;
		std::vector<struct sRobot> robots;


		class CAgent *collective_agent;

		float state_density;

		std::vector<float> start_position, target_position;


		class CMap *map;

		bool visualisation_enabled;
		u32 loops;

	public:
		CEnvironment(u32 agents_count, class CAgent *collective_agent = NULL, bool visualisation_enabled = NULL);
		~CEnvironment();


		void process();
		void print(std::vector<float> subspace);

		class CAgent* get_agent(u32 id);


	private:
		void respawn(struct sAgent *agent);
};

#endif

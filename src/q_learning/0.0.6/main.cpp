#include "common.h"
#include "environment.h"


int main()
{
	srand(time(NULL));


	struct sAgentInit agent_init;
	agent_init.id = 0;
	agent_init.type = 0;
	agent_init.dt = 1.0/32.0;;
	agent_init.gamma = 0.99; //0.98;
	agent_init.alpha = 0.9;
	agent_init.k = 1.0;
	agent_init.function_type = 0;
	agent_init.inputs_count = 2;

	std::vector<float> action;
	std::vector<std::vector<float>> actions;

	action.push_back(0.0);
	action.push_back(0.0);

	action[0] = 1.0;
	action[1] = 0.0;
	actions.push_back(action);

	action[0] = -1.0;
	action[1] = 0.0;
	actions.push_back(action);

	action[0] = 0.0;
	action[1] = 1.0;
	actions.push_back(action);

	action[0] = 0.0;
	action[1] = -1.0;
	actions.push_back(action);


	action[0] = 1.0;
	action[1] = 1.0;
	actions.push_back(action);

	action[0] = 1.0;
	action[1] = -1.0;
	actions.push_back(action);

	action[0] = -1.0;
	action[1] = 1.0;
	actions.push_back(action);

	action[0] = -1.0;
	action[1] = -1.0;
	actions.push_back(action);


	class CEnvironment *environment = new CEnvironment(agent_init, actions);


	environment->process();

	delete environment;


	printf("program done\n");
	return 0;
}

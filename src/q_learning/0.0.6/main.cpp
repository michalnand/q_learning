#include "common.h"
#include "environment.h"

void create_maps(float dt)
{
	u32 i;
	u32 fields_count = 0;

	class CMap *map;
	for (i = 0; i < MAPS_COUNT; i++)
	{
		char file_name[1024];
		sprintf(file_name,"%s/%s%i/map.bin", S_RESULTS_PATH, S_MAP_PATH, i);

		fields_count = 5.0*i*0.01/(dt*dt);
		map = new CMap(dt, fields_count);
		map->save(file_name);

		delete map;
	}
}

int main()
{
	srand(time(NULL));

	float dt = 1.0/32.0;
	struct sAgentInit agent_init;
	agent_init.id = 0;
	agent_init.type = 0;
	agent_init.dt = dt;
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


//	create_maps(dt);

	u32 res, i;

	std::vector<class CMap*> maps;

	for (i = 0; i < MAPS_COUNT; i++)
	{
		char source_file_name[1024];
		char dest_file_name[1024];

		sprintf(source_file_name,"%s/%s%i/map.bin", S_RESULTS_PATH, S_MAP_PATH, i);
		sprintf(dest_file_name,"%s/%s%i/map_plot.log", S_RESULTS_PATH, S_MAP_PATH, i);


		maps.push_back(new CMap(dt, 0));
		res = maps[i]->load(source_file_name);
		maps[i]->save_plot(dest_file_name);

		printf("loading %s with %i\n", source_file_name, res);
	}

	u32 map_id, function_type;

	printf("\n processing experiment : \n");

	class CLog log((char*)"results/results.log", 3);

	function_type = 0;
	//	map_id = 1;

	//for (function_type = 0; function_type < 4; function_type++)
	for (map_id = 2; map_id < MAPS_COUNT; map_id++)

	{
		float score;

		agent_init.function_type = function_type;
		class CEnvironment *environment = new CEnvironment(agent_init, actions, maps[map_id]);
		score = environment->process(1000000, map_id);
		delete environment;

		log.add(0, function_type);
		log.add(1, map_id);
		log.add(2, score);
	}

	log.save();

	printf("program done\n");
	return 0;
}

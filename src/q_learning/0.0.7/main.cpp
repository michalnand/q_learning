#include "common.h"
#include "environment.h"


void knn_test()
{
	struct KNNLayerInitStructure knn_init;

	class CKohonenLayer *knn;
	knn_init.inputs_count = 2;
	knn_init.neurons_count = 4*4;
	knn_init.weight_range = 1.0;
	knn_init.learning_constant = 0.8;
	knn_init.output_limit = 1.0;
	knn_init.outputs_count = 2;

	knn = new CKohonenLayer(knn_init);

	u32 i;
	for (i = 0; i < 100000; i++)
	{
		std::vector<float> input;

		input.push_back(rnd_());
		input.push_back(rnd_());

		float f0 = 0.0;
		float f1 = 0.0;

		if (sgn(input[0]) == sgn(input[1]))
			f0 = 1.0;
		else
			f0 = -1.0;

		f1 = cos(input[0]*input[1]*10.0);

		knn->process(input);
		knn->learn(NULL);
		knn->learn_single_output(f0, 0);
		knn->learn_single_output(f1, 1);
	}

	knn->save((char*)"weights.txt");
}

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
		map = new CMap(dt, fields_count, false);
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
	agent_init.gamma = 0.9;
	agent_init.alpha = 0.8;
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

//	 create_maps(dt);

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

	//class CLog log((char*)"results/map_1_results.log", 4);
	class CLog log((char*)"results/tmp_results.log", 4);

	map_id = 1;
	i = 0;
	function_type = 2;


	// for (map_id = 0; map_id < MAPS_COUNT; map_id++)

	for (i = 0; i < 10; i++)
//  for (function_type = 0; function_type < 2; function_type++)
	{
		float score;

		agent_init.function_type = function_type;
		class CEnvironment *environment = new CEnvironment(agent_init, actions, maps[map_id]);
		score = environment->process(1000000, map_id);
		delete environment;

		float tmp = log10(0.01 + score);

		log.add(0, function_type);
		log.add(1, i);
		log.add(2, score);
		log.add(3, tmp);

		log.save();
	}


	printf("program done\n");
	return 0;
}

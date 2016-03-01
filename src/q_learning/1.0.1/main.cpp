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
		map = new CMap(dt, fields_count, false);
		map->save(file_name);

		delete map;
	}
}


float compare_results(u32 map_id, u32 function_type)
{
	char required_value_file_name[1024];
	char approximated_value_file_name[1024];
	char error_result_log_file_name[1024];

	sprintf(required_value_file_name,"%s/map_%u/function_type_0/iterations_10/q_learning_result.log", S_RESULTS_PATH, map_id);
	sprintf(approximated_value_file_name,"%s/map_%u/function_type_%u/iterations_10/q_learning_result.log", S_RESULTS_PATH, map_id, function_type);
	sprintf(error_result_log_file_name,"%s/map_%u/function_type_%u/q_learning_error.log", S_RESULTS_PATH, map_id, function_type);


	CLogRead required_value(required_value_file_name);
	CLogRead approximated_value(approximated_value_file_name);
	CLog 		 error_result_log(error_result_log_file_name, 3);

	u32 i, j;
	float error = 0.0;
	for (j = 0; j < required_value.get_lines_count(); j++)
	{
		float error_line = 0.0;
		for (i = 0; i < 8; i++)
			error_line+= pow(required_value.get_item(j, i + 4) - approximated_value.get_item(j, i + 4), 2.0);

		error_result_log.add(0, required_value.get_item(j, 0));
		error_result_log.add(1, required_value.get_item(j, 1));
		error_result_log.add(2, error_line);

		error+= error_line;
	}

	error_result_log.save();

	return error;
}



void compare_all_results(u32 map_id)
{
	char function_1_file_name[1024];
	char function_2_file_name[1024];
	char function_3_file_name[1024];
	char function_4_file_name[1024];

	char trials_average_results_file_name[1024];

	sprintf(function_1_file_name,"%s/map_%u/function_type_1/summary_error_results.log", S_RESULTS_PATH, map_id);
	sprintf(function_2_file_name,"%s/map_%u/function_type_2/summary_error_results.log", S_RESULTS_PATH, map_id);
	sprintf(function_3_file_name,"%s/map_%u/function_type_3/summary_error_results.log", S_RESULTS_PATH, map_id);
	sprintf(function_4_file_name,"%s/map_%u/function_type_4/summary_error_results.log", S_RESULTS_PATH, map_id);

	sprintf(trials_average_results_file_name,"%s/map_%u/trials_average_results.log", S_RESULTS_PATH, map_id);


	std::vector<CLogRead *> functions;

	functions.push_back(new CLogRead(function_1_file_name));
	functions.push_back(new CLogRead(function_2_file_name));
	functions.push_back(new CLogRead(function_3_file_name));
	functions.push_back(new CLogRead(function_4_file_name));

	CLog 		 trials_average_results(trials_average_results_file_name, 4);

	u32 i, j;
	for (j = 0; j < functions.size(); j++)
	{
		float error_min = 1000000000.0;
		float error_max = -error_min;
		float error_average = 0.0;

		for (i = 0; i < functions[j]->get_lines_count(); i++)
		{
			float error = functions[j]->get_item(i, 1);

			error_average+= error;

			if (error > error_max)
				error_max = error;

			if (error < error_min)
				error_min = error;
		}

		error_average/= functions[j]->get_lines_count();

		trials_average_results.add(0, j);
		trials_average_results.add(1, error_average);
		trials_average_results.add(2, error_min);
		trials_average_results.add(3, error_max);
	}

	trials_average_results.save();
}

int main()
{
	srand(time(NULL));


	float dt = 1.0/32.0;
	struct sAgentInit agent_init;
	agent_init.id = 0;
	agent_init.type = 0;
	agent_init.dt = dt;
	agent_init.gamma = 0.98;
	agent_init.alpha = 0.7;
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

	float error = 0.0;
	map_id = 1;
	i = 0;
	function_type = 4;
/*
//	 for (map_id = 0; map_id < MAPS_COUNT; map_id++)

 	//for (function_type = 0; function_type < 5; function_type++)
	{
		char file_name[1024];
		sprintf(file_name,"%s/map_%u/function_type_%u/summary_error_results.log",S_RESULTS_PATH, map_id, function_type);
		class CLog log(file_name, 2);

		for (i = 0; i < 20; i++)
		{
			float score;

			agent_init.function_type = function_type;
			class CEnvironment *environment = new CEnvironment(agent_init, actions, maps[map_id]);
			score = environment->process(map_id);

			error = compare_results(map_id, function_type);
			delete environment;

			printf("total error %f\n", error);

			log.add(0, i);
			log.add(1, error);

			log.save();
		}
	}
	*/

	compare_all_results(1);


	printf("program done\n");
	return 0;
}

#include "environment.h"

extern struct sVisualisation g_visualisation;

CEnvironment::CEnvironment(u32 agents_count, class CAgent *collective_agent, bool visualisation_enabled)
{
    this->visualisation_enabled = visualisation_enabled;


 	u32 j,i ;

 	u32 outputs_count = 2;
 	u32 agent_inputs_count = 2;
 	u32 actions_per_state = 4;

 	state_density = 1.0/10.0;

 	u32 id = 0;
 	u32 type = AGENT_TYPE_NULL;

 	s_agent_init(
				 	&agent_init,
				 	id, type,
					outputs_count, agent_inputs_count,
					actions_per_state,
					state_density
				);


    map = new CMap(0, 0, 34, 19, 55.0, 55.0);

    //if (map->load((char*)"map_targets.txt") == 0)
    //if (map->load((char*)"q_learning_test_00.txt") == 0)
    if (map->load((char*)"q_learning_test_01.txt") == 0)
    {
        printf("map success loaded\n");
    }
    else
    {
        printf("map loading failed\n");
    }

    if (visualisation_enabled == false)
    {
        printf("\nmap\n");
        map->print();
    }

    // sleep(2);


 	//this robot is for sharing solution
 	//collective_agent = NULL;
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


    if (visualisation_enabled == false)
    {
        printf("\nagent\n");

        std::vector<float> subspace;
        agents[0]->print(subspace);
    }


    float x, y;
    float sx = 0.0, sy = 0.0;
    float tx = 0.0, ty = 0.0;
    float delta = state_density/4.0;

    for (y = -1.0; y < 1.0; y+= delta)
    {
        for (x = -1.0; x < 1.0; x+= delta)
        {
            if (map->get_at_normalised(x, y).type == 3)
            {
                tx = x;
                ty = y;
            }

            if (map->get_at_normalised(x, y).type == 9)
            {
                sx = x;
                sy = y;
            }
        }
    }

    target_position.push_back(tx);
    target_position.push_back(ty);

    loops = 0;



    if (visualisation_enabled == true)
    {
        g_visualisation.window_width = VISUALISATION_SCREEN_WIDTH;
        g_visualisation.window_height = VISUALISATION_SCREEN_HEIGHT;

        g_visualisation.base_size = 1.0/1.0; //VISUALISATION_ROBOT_SIZE;


        g_visualisation.position_max_x = 1.0;
        g_visualisation.position_max_y = 1.0;
        g_visualisation.position_max_z = 2.0;

        visualisation_init();

        u32 robot_id = 0;
        for (j = 0; j < agents.size(); j++)
        {
            struct sRobot robot;
            robot.id = /* cfg_get_id() + */ robot_id + 1;
            robot.type = ROBOT_TYPE_COMMON;
            robot.request = REQUEST_NULL;
            robot.parameter = 0;

            robot.dt = 1.0/100.0;   //dt in ms
            robot.time = 0.0;

            robot_id++;

            for (i = 0; i < ROBOT_SENSORS_COUNT; i++)
                robot.sensors[i] = 0.0;

            for (i = 0; i < ROBOT_SPACE_DIMENSION; i++)
            {
                robot.d[i] = 0.0;
                robot.position[i] = s_agents[j].state[i];
                robot.angles[i] = 0.0;
            }

            robots.push_back(robot);
        }

        visualisation_update_all(&robots);
    }

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

		float target_dist  = abs_(target_position[0] - s_agents[j].state[0]) +
                             abs_(target_position[1] - s_agents[j].state[1]);

		float reward = map->get_at_normalised(s_agents[j].state[0], s_agents[j].state[1]).reward;


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

        if ((loops%1000) == 0)
        {
            s_agents[j].reward = 0.0;
            respawn(&s_agents[j]);
        }

		if (target_dist < target_min_dist)
		{
            if (visualisation_enabled == false)
            {
    			printf("robot %u on target, score %f\n", j, s_agents[j].score);

                std::vector<float> subspace;
                agents[j]->print(subspace);
            }

			s_agents[j].reward = 0.0;
			respawn(&s_agents[j]);
		}
		else
		{
            for (i = 0; i < s_agents[j].state.size(); i++)
            {
			    s_agents[j].state[i] = s_agents[j].state[i] +
									s_agents[j].output_action.action[i]*s_agents[j].state_density;

                s_agents[j].state[i] = saturate(s_agents[j].state[i], -1.0, 1.0);
            }
		}
	}

    loops++;

    if (visualisation_enabled == true)
    {
        for (j = 0; j < agents.size(); j++)
        for (i = 0; i < ROBOT_SPACE_DIMENSION; i++)
        {
            robots[j].d[i] = 0.0;
            robots[j].position[i] = s_agents[j].state[i];
            robots[j].angles[i] = 0.0;
        }

        visualisation_update_all(&robots);
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

class CAgent* CEnvironment::get_agent(u32 id)
{
    return agents[id];
}

void CEnvironment::respawn(struct sAgent *agent)
{
	u32 i;


   do
    {
    	for (i = 0; i < agent->inputs_count; i++)
    		agent->state[i] =  rnd_(); //agent to random place
    }
    while (map->get_at_normalised(agent->state[0], agent->state[1]).type != 0);

	agent->score = 0.0;
}

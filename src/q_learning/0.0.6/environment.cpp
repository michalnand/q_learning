#include "environment.h"



CEnvironment::CEnvironment(struct sAgentInit agent_init, std::vector<std::vector<float>> actions)
{
    this->agent_init = agent_init;


    dt = this->agent_init.dt;
    //u32 fields_count = 0.05/(dt*dt);
    u32 fields_count = 0.1/(dt*dt);

    map = new CMap(dt, fields_count);


    map->save((char*)"map_log/map.bin");
  //  map->load((char*)"map_log/map.bin");
    map->save_plot((char*)"map_log/map_plot_log.log");



    this->agent_init.actions = actions;


    agent = new CAgent(this->agent_init);
}

CEnvironment::~CEnvironment()
{
  delete map;
  delete agent;
}

float CEnvironment::process_one_run(char *path_name, u32 process_best, u32 max_runs, i32 id)
{
  u32 i;
  struct sMapField map_field;

  std::vector<float> agent_input;
  agent_input.push_back(0.0);
  agent_input.push_back(0.0);

  float x = rnd_(), y = rnd_(), score = 0.0;

  CLog *log = NULL;

  if (path_name != NULL)
  {
    char file_name[1024];
    sprintf(file_name,"%s/agent_path_%i.log", path_name, id);
    log = new CLog(file_name, 3);
  }

  for (i = 0; i < max_runs; i++)
  {
    agent_input[0] = x;
    agent_input[1] = y;

    map_field = map->get(x, y, 0.0);
    float reward = map_field.reward;

    u32 learn;
    if (process_best != 0)
      learn = 0;
    else
      learn = 1;

    agent->process(agent_input, reward, learn);


    struct sAgentRes agent_res;
    agent_res = agent->get();



    if (process_best != 0)
    {
      x+= dt*agent_res.best_action[0];
      y+= dt*agent_res.best_action[1];
      score+= agent_res.q_res.best_action_q;
    }
    else
    {
      x+= dt*agent_res.action[0];
      y+= dt*agent_res.action[1];
      score+= agent_res.q_res.selected_action_q;
    }


    if (x > 1.0)
      x = 1.0;
    if (x < -1.0)
      x = -1.0;

    if (y > 1.0)
      y = 1.0;
    if (y < -1.0)
      y = -1.0;



    if (log != NULL)
    {
      log->add(0, x);
      log->add(1, y);
      log->add(2, 0);
    }

    if (map_field.type == MAP_FIELD_TARGET)
    {
      x = rnd_();
      y = rnd_();
      agent->reset();
    }

    if ((map_field.type == MAP_FIELD_TARGET) && (process_best != 0))
      break;
  }

  if (log != NULL)
  {
    log->save();
  }

  if (path_name != NULL)
  {
    char file_name[1024];
    sprintf(file_name,"%s/q_func_best_value_log.log", path_name);
    agent->save_best_q_plot(agent_input, file_name);
  }


  return score;
}

void CEnvironment::process()
{
  process_one_run(NULL, 0, 1000000, 0);

  u32 i;
  u32 testing_iterations_count = 1000;
  float testing_score = 0.0;


  for (i = 0; i < testing_iterations_count; i++)
  {
    if (i < 100)
      testing_score+= process_one_run((char*)"q_func_log", 1, 10000, i);
    else
      testing_score+= process_one_run(NULL, 1, 10000, i);
  }

  testing_score/= testing_iterations_count;

  printf("%f\n", testing_score);


  process_one_run((char*)"q_func_log", 1, 1000, -1);

}

#include "environment.h"



CEnvironment::CEnvironment(struct sAgentInit agent_init, std::vector<std::vector<float>> actions, class CMap *map)
{
    this->agent_init = agent_init;
    this->dt = this->agent_init.dt;

    this->map = map;

    this->agent_init.actions = actions;
    this->agent = new CAgent(this->agent_init);
}

CEnvironment::~CEnvironment()
{
  delete agent;
}

void CEnvironment::process_learn_run(u32 learning_iterations)
{
  u32 i;
  struct sMapField map_field;

  std::vector<float> agent_input;
  agent_input.push_back(0.0);
  agent_input.push_back(0.0);

  float x = rnd_();
  float y = rnd_();

  for (i = 0; i < learning_iterations; i++)
  {
    agent_input[0] = x;
    agent_input[1] = y;

    map_field = map->get(x, y, 0.0);
    float reward = map_field.reward;

    agent->process(agent_input, reward, 1);

    struct sQlearningRes q_res;
    q_res = agent->get();

    x+= dt*q_res.action[0];
    y+= dt*q_res.action[1];

    if (x > 1.0)
      x = 1.0;
    if (x < -1.0)
      x = -1.0;

    if (y > 1.0)
      y = 1.0;
    if (y < -1.0)
      y = -1.0;

    if (map_field.type == MAP_FIELD_TARGET)
    {
      x = rnd_();
      y = rnd_();

      agent->process(agent_input, reward, 1, 1);
    }
  }
}

float CEnvironment::process_test_run(u32 iterations, u32 id, char *path_name)
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

  for (i = 0; i < iterations; i++)
  {
    agent_input[0] = x;
    agent_input[1] = y;

    map_field = map->get(x, y, 0.0);
    float reward = map_field.reward;

    agent->process(agent_input, reward, 0);


    struct sQlearningRes q_res;
    q_res = agent->get();

    x+= dt*q_res.action_best[0];
    y+= dt*q_res.action_best[1];

    if (reward < 0.0)
      score+= reward;


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
      agent->process(agent_input, reward, 0);
    }
  }

  if (log != NULL)
  {
    log->save();
    delete log;
  }

  if (path_name != NULL)
  {
    char file_name[1024];
    sprintf(file_name,"%s/q_learning_result.log", path_name);
    agent->save_q(agent_input, file_name);
  }

  return score;
}

float CEnvironment::process(u32 map_id)
{
  u32 i;
  u32 iterations;

  u32 learning_iterations = 50000;

  char result_path[1024];

  for (iterations = 1; iterations <= 10; iterations++)
  {
    sprintf(result_path,"%s/%s%u/function_type_%u/iterations_%u", S_RESULTS_PATH, S_MAP_PATH, map_id, agent_init.function_type, iterations);

    #if DEBUG_MODE==1
    printf("> %s\n", result_path);
    printf("   learning\n");
    #endif

    process_learn_run(learning_iterations);

    agent->save();

    #if DEBUG_MODE==1
    printf("   testing\n");
    #endif

    //save robots paths for plotting
    for (i = 0; i < 50; i++)
      process_test_run(256, i, result_path);
  }

  //now maximum learning iterations done
  //process 100 runs to take bests results
  float score = 0.0;
  for (i = 0; i < 1000; i++)
    score+= process_test_run(256, i, NULL);
  score = score / 1000.0;
  printf("done with score %f\n", score);

  return score;
}

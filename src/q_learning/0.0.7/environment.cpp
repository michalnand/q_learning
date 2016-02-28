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

//float CEnvironment::process_one_run(char *path_name, u32 process_best, u32 max_runs, i32 id, )

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

  float targets_hits = 0.001;
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

    score+= (reward + 1.0)/2.0;

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

      targets_hits+= 1.0;
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
    agent->save_best_q_plot(agent_input, file_name);
  }

  score = score*targets_hits;

  return score;
}

float CEnvironment::process(u32 learning_iterations, u32 map_id)
{
  u32 i, j;
  u32 iterations;

  char result_path[1024];
  sprintf(result_path,"%s/%s%u/function_type_%u/score_summary.log", S_RESULTS_PATH, S_MAP_PATH, map_id, agent_init.function_type);
  printf("> summary saved into %s\n", result_path);


  class CLog log(result_path, 3);

  for (iterations = 1; iterations <= 10; iterations++)
  {
    sprintf(result_path,"%s/%s%u/function_type_%u/iterations_%u", S_RESULTS_PATH, S_MAP_PATH, map_id, agent_init.function_type, iterations);
    printf("> %s\n", result_path);

    printf("   learning\n");
    process_learn_run(50000);

    agent->save();

    printf("   testing\n");
    //save robots paths for plotting
    for (i = 0; i < 50; i++)
      process_test_run(256, i, result_path);

    //process 30times 100 testing runs
    for (j = 0; j < 30; j++)
    {
      float partial_score = 0.0;
      u32 tmp = 10;
      for (i = 0; i < tmp; i++)
        partial_score+= process_test_run(256, i, NULL);


      partial_score = partial_score/tmp;

      log.add(0, iterations);
      log.add(1, j);
      log.add(2, partial_score);
    }
  }

  log.save();

  //now maximum learning iterations done
  //process 100 runs to take bests results
  float score = 0.0;
  for (i = 0; i < 1000; i++)
    score+= process_test_run(256, i, NULL);

  score = score / 1000.0;

  printf("done with score %f\n",score);

  return score;
}

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "robot_brain/agent.h"
#include "map.h"

class CEnvironment
{
  private:
    float dt;
    class CMap *map;

    struct sAgentInit agent_init;
    class CAgent *agent;

  public:
    CEnvironment(struct sAgentInit agent_init, std::vector<std::vector<float>> actions, class CMap *map);
    ~CEnvironment();

    float process(u32 map_id);

  private:
  //  float process_one_run(char *log_file_name, u32 process_best, u32 max_runs, i32 id);
  void process_learn_run(u32 learning_iterations);
  float process_test_run(u32 iterations, u32 id, char *path_name);


};

#endif

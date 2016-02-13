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
    CEnvironment(struct sAgentInit agent_init, std::vector<std::vector<float>> actions);
    ~CEnvironment();

    void process();

  private:
    float process_one_run(char *log_file_name, u32 process_best, u32 max_runs, i32 id);

};

#endif

#ifndef _AGENT_H_
#define _AGENT_H_

#include "q_learning.h"



struct sAgentInit
{
  u32 id;
  u32 type;

  float dt;
  float gamma, alpha, k;
  u32 function_type;

  u32 inputs_count;
  std::vector<std::vector<float>> actions;
};

struct sAgentRes
{
  struct sQlearningRes q_res;
  std::vector<float> action, best_action;
};

class CAgent
{
  private:
    class CQlearning *q_learning;
    struct sAgentInit agent_init;
    struct sAgentRes agent_res;

  public:
    CAgent(struct sAgentInit ag_init_struct);
    ~CAgent();

    void process(std::vector<float> input, float reward, u32 learn);

    void reset();
    struct sAgentRes get();

    void save_best_q_plot(std::vector<float> subspace, char *file_name);
};



#endif

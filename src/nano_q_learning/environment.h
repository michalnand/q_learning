#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "nano_q.h"

struct sEnvironmentResult
{
  float average_error;
  float sigma_error;
};

class CEnvironment
{
  private:
    float dt, noise;
    bool log_enabled;

    std::vector<std::vector<float>> actions;

    std::vector<std::vector<float>> robots_positions;

    std::vector<float> target_distance_prev;
    std::vector<float> target_position;

    std::vector<struct sNanoQ> nq;

    class CLog *result_log, *score_log;
  public:
    CEnvironment(float dt, float gamma, float noise, u32 robots_count, bool log_enabled);
    ~CEnvironment();

    void process(u32 iterations_max);

    void log_add();
    void save();

    struct sEnvironmentResult get_result();

};


#endif

#include "environment.h"



/*
    std::vector<std::vector<float>> robots_positions;

    std::vectorstruct sNanoQ> nq;
*/

CEnvironment::CEnvironment(float dt, float gamma, float noise, u32 robots_count, bool log_enabled)
{
  u32 j;

  this->dt = dt;
  this->noise = noise;
  this->log_enabled = log_enabled;

  std::vector<float> action;
  action.push_back(0.0);
  action.push_back(0.0);

  action[0] = dt;
  action[1] = 0.0;
  actions.push_back(action);

  action[0] = -dt;
  action[1] = 0.0;
  actions.push_back(action);


  action[0] = 0.0;
  action[1] = dt;
  actions.push_back(action);

  action[0] = 0.0;
  action[1] = -dt;
  actions.push_back(action);

/*
  action[0] = dt;
  action[1] = dt;
  actions.push_back(action);

  action[0] = -dt;
  action[1] = dt;
  actions.push_back(action);


  action[0] = dt;
  action[1] = -dt;
  actions.push_back(action);

  action[0] = -dt;
  action[1] = -dt;
  actions.push_back(action);
*/

  std::vector<float> robot_position;
  robot_position.push_back(0.0);
  robot_position.push_back(0.0);

  target_position.push_back(0.0);
  target_position.push_back(0.0);


  for (j = 0; j < robots_count; j++)
  {
      robot_position[0] = rnd_();
      robot_position[1] = rnd_();

      robots_positions.push_back(robot_position);

      struct sNanoQ nq_tmp;
      nano_q_init(&nq_tmp, gamma*NANO_Q_GAMMA_MAX);

      nq.push_back(nq_tmp);

      float distance_max = robots_positions[j].size()*robot_position.size()*4.0;
      target_distance_prev.push_back(distance_max);
  }

  result_log = NULL;
  score_log = NULL;

  if (log_enabled)
  {
    result_log = new CLog((char*)"result.log",2*(1+robots_count));
    score_log = new CLog((char*)"score.log",robots_count);
  }
}

CEnvironment::~CEnvironment()
{
  if (log_enabled)
  {
    delete result_log;
    delete score_log;
  }
}


void CEnvironment::process(u32 iterations_max)
{
  u32 iterations;

  float t = 0.0;
  for (iterations = 0; iterations < iterations_max; iterations++)
  {
    t+= dt/1.1; //(2.0*PI*iterations)/iterations_max;
    target_position[0] = 0.7*cos(t);
    target_position[1] = 0.7*sin(t);

    u32 j, i;
    for (j = 0; j < nq.size(); j++)
    {
      nano_q_select_action_id_best(&nq[j]);

      u32 action_id = nq[j].action_id;

      for (i = 0; i < actions[action_id].size(); i++)
      {
        robots_positions[j][i]+= actions[action_id][i];

        if (robots_positions[j][i] > 1.0)
          robots_positions[j][i] = 1.0;

        if (robots_positions[j][i] < -1.0)
          robots_positions[j][i] = -1.0;
      }

      float target_distance = 0.0;
      for (i = 0; i < robots_positions[j].size(); i++)
        target_distance+= pow(target_position[i] - robots_positions[j][i], 2.0);


      float reward = sgn(target_distance_prev[j] - target_distance);
      target_distance_prev[j] = target_distance;

      if ((rand()%100) < (noise*100))
        reward*= -1;

      nano_q_process(&nq[j], reward);
    }

    if (log_enabled)
    {
      log_add();
    }
  }

}


void CEnvironment::log_add()
{
  u32 j;

  result_log->add(0*3 + 0, target_position[0]);
  result_log->add(0*3 + 1, target_position[1]);

  for (j = 0; j < nq.size(); j++)
  {
    float x = robots_positions[j][0];
    float y = robots_positions[j][1];
    float distance = target_distance_prev[j];

  //  printf("[%6.3f %6.3f %6.3f] ", x, y, distance);

    result_log->add((j+1)*2 + 0, x);
    result_log->add((j+1)*2 + 1, y);

    score_log->add(j*1 + 0, distance);
  }

  //printf("\n");
}

void CEnvironment::save()
{
  result_log->save();
  score_log->save();
}

struct sEnvironmentResult CEnvironment::get_result()
{
  struct sEnvironmentResult res;

  res.average_error = 0.0;
  res.sigma_error = 0.0;

  u32 j;

  for (j = 0; j < nq.size(); j++)
    res.average_error+= target_distance_prev[j];

  res.average_error/= nq.size();

  float tmp = 0.0;
  for (j = 0; j < nq.size(); j++)
    tmp+= (1.0/nq.size())*pow(res.average_error - target_distance_prev[j], 2.0);

  res.sigma_error = sqrt(tmp);

  return res;
}

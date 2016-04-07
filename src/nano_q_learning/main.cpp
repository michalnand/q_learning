#include "environment.h"

int main()
{
  srand(time(NULL));

  class CEnvironment *environment, *environment_tmp;


  float dt = 0.001;
  float gamma = 0.9;
  float noise = 0.3;
  u32 robots_count = 32;
  u32 iterations_max = 22000;

  environment_tmp = new CEnvironment(dt, gamma, noise, robots_count, true);
  environment_tmp->process(iterations_max);
  environment_tmp->save();
  delete environment_tmp;

  return 0;

  class CLog *summary_result_log;
  summary_result_log = new CLog((char*)"summary_result.log", 4);

  for (gamma = 0.0; gamma <= 0.9; gamma+= 0.9/40.0)
  for (noise = 0.0; noise <= 0.4; noise+= 0.4/40.0)
  {
    environment = new CEnvironment(dt, gamma, noise, robots_count, false);

    environment->process(iterations_max);

  //  environment->save();

    struct sEnvironmentResult result;

    result = environment->get_result();

    delete environment;

    summary_result_log->add(0, gamma);
    summary_result_log->add(1, noise);
    summary_result_log->add(2, result.average_error);
    summary_result_log->add(3, result.sigma_error);

    printf("%6.3f %6.3f > %6.3f %6.3f\n", gamma, noise, result.average_error, result.sigma_error);
  }

  summary_result_log->save();

	return 0;
}

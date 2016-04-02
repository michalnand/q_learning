#include "common.h"


struct sPeakHill
{
  std::vector<std::vector<float>> p_alpha;
  std::vector<float> p_r;

  std::vector<float> h_beta, h_w;
  std::vector<std::vector<float>> h_alpha;
};


void peak_hill_random(struct sPeakHill *func, u32 dim, u32 peak_count, u32 hill_count)
{
  u32 i, j;
  std::vector<float> tmp;
  for (i = 0; i < dim; i++)
    tmp.push_back(0.0);

  for (j = 0; j < peak_count; j++)
  {
    for (i = 0; i < dim; i++)
      tmp[i] = rnd_();
    func->p_alpha.push_back(tmp);
    func->p_r.push_back(-4.0*abs_(rnd_()));
  }

  for (j = 0; j < hill_count; j++)
  {
    for (i = 0; i < dim; i++)
      tmp[i] = rnd_();
    func->h_alpha.push_back(tmp);
    func->h_beta.push_back(10.0 + 10.0*abs_(rnd_()));
    func->h_w.push_back(abs_(rnd_()));
  }
}

float peak_hill_process(struct sPeakHill *func, std::vector<float> input)
{
  u32 j, i;
  float res = 0.0;
  for (j = 0; j < func->p_alpha.size(); j++)
  {
    float dist = 0.0;
    for (i = 0; i < func->p_alpha[j].size(); i++)
      dist+= pow((input[i] - func->p_alpha[j][i]), 2.0);

    if (dist < 0.001)
      res+= func->p_r[j];
  }

  for (j = 0; j < func->h_alpha.size(); j++)
  {
    float sum = 0.0;
    for (i = 0; i < func->h_alpha[j].size(); i++)
      sum+= pow((input[i] - func->h_alpha[j][i]), 2.0);

    res+= func->h_w[j]*exp(-func->h_beta[j]*sum);
  }

  return res;
}


int main()
{
  class CLog output_log((char*)"peak_hill_function.log", 3);

  u32 peak_count = 30;
  u32 hill_count = 20;

  struct sPeakHill func;
  peak_hill_random(&func, 2, peak_count, hill_count);

  float x, y, dt = 1.0/32.0;

  std::vector<float> input;
  input.push_back(0.0);
  input.push_back(0.0);

  for (y = -1.0; y < 1.0; y+= dt)
  for (x = -1.0; x < 1.0; x+= dt)
  {
    input[0] = x;
    input[1] = y;
    float res = peak_hill_process(&func, input);

    output_log.add(0, input[0]);
    output_log.add(1, input[1]);
    output_log.add(2, res);
  }

  output_log.save();
	printf("program done\n");

	return 0;
}

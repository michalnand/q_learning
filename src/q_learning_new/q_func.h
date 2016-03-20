#ifndef _Q_FUNC_H_
#define _Q_FUNC_H_

class CQfunc
{
  private:
    u32 functions_count, function_dimension;
    u32 winning_neuron;
    float *w;
    float *beta;
    float *h;
    float **alpha;
    float *d;
};



void process(std::vector<float> input)
{
  u32 j;

  winning_neuron = 0;
  for (j = 0; j < functions_count; j++)
  {
    d[j] = 0.0;
    for (i = 0; i < function_dimension; i++)
      d[j]+= (alpha[j][i] - input[i])*(alpha[j][i] - input[i]);

    if (d[j] < d[winning_neuron])
      winning_neuron = j;
  }

  float res = 0.0;

  res+= h[winning_neuron];
  for (j = 0; j < functions_count; j++)
    res+= w*exp(-beta[j]*d[j]);

  q_res = res;
  return q_res;
}

void learn(std::vector<float> input, float reward_value, float q_rest_value)
{
  process(input);
  u32 i;

  //move to centers
  for (i = 0; i < function_dimension; i++)
    alpha[winning_neuron][i] =  (1.0 - eta_1)*alpha[winning_neuron][i] + eta_1*input[i];


  //estimate H function value
  float error_reward = reward_value - h[winning_neuron];

  h[winning_neuron]+= eta_2*error_reward;
  if (h[winning_neuron] > 1.0)
    h[winning_neuron] = 1.0;

  if (h[winning_neuron] < -1.0)
    h[winning_neuron] = -1.0;

  //estimate E function value
  float error_rest = q_rest_value - q_res;

  for (j = 0; j < functions_count; j++)
  {
    
  }
}

#endif

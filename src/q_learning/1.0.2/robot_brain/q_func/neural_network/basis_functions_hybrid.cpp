#include "basis_functions_hybrid.h"

CBasisFunctionsHybrid::CBasisFunctionsHybrid(u32 count_peek, u32 count_hill, u32 dimension)
{
  u32 j, i;

  this->count_peek = count_peek;
  this->count_hill = count_hill;
  this->dimension = dimension;

  for (i = 0; i < this->dimension; i++)
    this->input.push_back(0.0);

  peak_value = (float*)malloc(this->count_peek*sizeof(float));
  peak_centre = (float**)malloc(this->count_peek*sizeof(float*));
  for (j = 0; j < this->count_peek; j++)
  {
    peak_value[j] = 0.0;

    peak_centre[j] = (float*)malloc(this->dimension*sizeof(float));
    for (i = 0; i < this->dimension; i++)
      peak_centre[j][i] = rnd_();
  }

  hill_value = (float*)malloc(this->count_hill*sizeof(float));
  hill_shape = (float*)malloc(this->count_hill*sizeof(float));
  hill_centre = (float**)malloc(this->count_hill*sizeof(float*));
  hill_output = (float*)malloc(this->count_hill*sizeof(float));
  hill_distance = (float*)malloc(this->count_hill*sizeof(float));

  for (j = 0; j < this->count_hill; j++)
  {
    float k = 0.90;
    hill_shape[j] = 5; //200.0;
    //hill_shape[j] = 20 + 50.0*abs_(rnd_());
    //hill_shape[j] = 100.0*(k + (1.0 - k)*abs_(rnd_()));

    hill_centre[j] = (float*)malloc(this->dimension*sizeof(float));
    for (i = 0; i < this->dimension; i++)
      hill_centre[j][i] = rnd_();

    hill_distance[j] = 0.0;
    hill_output[j] = 0.0;
  }

  this->output = 0.0;

  peak_min_dist_idx = 0;
  hill_min_dist_idx = 0;

  peak_min_dist = 0.005;

  peak_min_dist_value = this->dimension*10.0;
  hill_min_dist_value = this->dimension*10.0;
}

CBasisFunctionsHybrid::~CBasisFunctionsHybrid()
{
  u32 j;
  free(peak_value);
  for (j = 0; j < this->count_peek; j++)
    free(peak_centre[j]);
  free(peak_centre);


  free(hill_value);
  free(hill_shape);
  for (j = 0; j < this->count_hill; j++)
    free(hill_centre[j]);
  free(hill_centre);

  free(hill_output);
  free(hill_distance);
}


void CBasisFunctionsHybrid::process(std::vector<float> input)
{
  u32 j, i;

  this->input = input;

  float res = 0.0;

  peak_min_dist_value = this->dimension*10.0;
  peak_min_dist_idx = 0;

  for (j = 0; j < this->count_peek; j++)
  {
    float dist = 0.0;
    for (i = 0; i < this->dimension; i++)
      dist+= pow(input[i] - peak_centre[j][i], 2.0);
    if (dist < peak_min_dist)
      res+= peak_value[j];

    if (dist < peak_min_dist_value)
    {
      peak_min_dist_value = dist;
      peak_min_dist_idx = j;
    }
  }


  hill_min_dist_value = this->dimension*10.0;
  hill_min_dist_idx = 0;

  for (j = 0; j < this->count_hill; j++)
  {
    float dist = 0.0;
    for (i = 0; i < this->dimension; i++)
      dist+= pow(input[i] - hill_centre[j][i], 2.0);

    hill_distance[j] = dist;

    float tmp = exp(-hill_shape[j]*dist);
    hill_output[j] = tmp;

    res+= hill_value[j]*tmp;

    if (dist < hill_min_dist_value)
    {
      hill_min_dist_value = dist;
      hill_min_dist_idx = j;
    }
  }

  output = res;
}

float CBasisFunctionsHybrid::get()
{
  return output;
}

void CBasisFunctionsHybrid::learn(float required_value, float learning_rate)
{
  u32 j, i;

  float error = required_value - output;

  if ((this->count_peek > 0) && (required_value < 0.0))
  {
    float lc_peak = learning_rate;
    float lc = 0.1*tanh(10.0*required_value*required_value);

    for (i = 0; i < this->dimension; i++)
      peak_centre[peak_min_dist_idx][i] = (1.0 - lc)*peak_centre[peak_min_dist_idx][i] +
                                                 lc*input[i];


    if (peak_min_dist_value < peak_min_dist)
    {
      peak_value[peak_min_dist_idx]+= lc_peak*error;

      if (peak_value[peak_min_dist_idx] > 1.0)
        peak_value[peak_min_dist_idx] = 1.0;
      if (peak_value[peak_min_dist_idx] < -1.0)
        peak_value[peak_min_dist_idx] = -1.0;
    }
  }

  if ((this->count_hill > 0) && (required_value > 0.0))
  {
    float lc_hill = learning_rate;
    float lc = 0.2*tanh(10.0*required_value*required_value);

    for (i = 0; i < this->dimension; i++)
      hill_centre[hill_min_dist_idx][i] = (1.0 - lc)*hill_centre[hill_min_dist_idx][i] +
                                                 lc*input[i];

    if (hill_min_dist_value < peak_min_dist)
    {
      hill_value[hill_min_dist_idx]+= 2.0*lc_hill*error;

      if (hill_value[hill_min_dist_idx] > 1.0)
        hill_value[hill_min_dist_idx] = 1.0;
      if (hill_value[hill_min_dist_idx] < -1.0)
        hill_value[hill_min_dist_idx] = -1.0;
    }

    for (j = 0; j < this->count_hill; j++)
    {
      float error = required_value - hill_output[j];
      hill_shape[j]+= -lc_hill*error;

      if (hill_shape[j] > 100.0)
        hill_shape[j] = 100.0;
      if (hill_shape[j] < 0.0)
        hill_shape[j] = 0.0;
    }
  }

    /*
    float lc_hill = learning_rate;
    float lc = 0.1*tanh(100.0*required_value*required_value);


    for (j = 0; j < this->count_hill; j++)
    {
      hill_value[j]+= 0.01*error*hill_output[j];

      if (hill_value[j] > 1.0)
        hill_value[j] = 1.0;

      if (hill_value[j] < 0.0)
        hill_value[j] = 0.0;

      hill_shape[j]+= lc_hill*100.0*error*hill_output[j];

      if (hill_shape[j] > 100.0)
        hill_shape[j] = 100.0;
      if (hill_shape[j] < 0.0)
        hill_shape[j] = 0.0;
    }

    for (i = 0; i < this->dimension; i++)
      hill_centre[hill_min_dist_idx][i] = (1.0 - lc)*hill_centre[hill_min_dist_idx][i] + lc*input[i];
    */

}

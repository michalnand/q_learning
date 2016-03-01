#include "basis_functions.h"

CBasisFunctions::CBasisFunctions(u32 count, u32 dimension, float a_range, float b_range, float w_range, bool multiply_network_type)
{
  this->functions_count = count;
  this->dimension = dimension;

  this->a_range = a_range;
  this->b_range = b_range;
  this->w_range = w_range;

  this->multiply_network_type = multiply_network_type;

  u32 j, i;

  a = (float**)malloc(this->functions_count*sizeof(float*));
  b = (float*)malloc(this->functions_count*sizeof(float));


  for (j = 0; j < this->functions_count; j++)
  {
    a[j] = (float*)malloc(this->dimension*sizeof(float));

    for (i = 0; i < this->dimension; i++)
      a[j][i] = this->a_range*rnd_();  //in range -a_range..a_range

    float k = 0.95;
    b[j] = this->b_range*(k + (1.0 - k)*abs_(rnd_()));
  }

  for (j = 0; j < this->functions_count; j++)
    output.push_back(0.0);

  w = (float*)malloc(this->functions_count*sizeof(float));
  distance = (float*)malloc(this->functions_count*sizeof(float));
  for (j = 0; j < this->functions_count; j++)
  {
    w[j] = 0.0;
    distance[j] = 0.0;
  }


  v = NULL;

  if (this->multiply_network_type == true)
  {
    printf("\n\nMULTIPLY\n\n");
  }

  for (i = 0; i < this->dimension; i++)
    input.push_back(0.0);

  linear_combination = 0.0;
}

CBasisFunctions::~CBasisFunctions()
{
  u32 j;
  for (j = 0; j < this->functions_count; j++)
  {
    free(a[j]);
  }

  if (v != NULL)
  {
    free(v);
    v = NULL;
  }

  free(a);
  free(b);
  free(w);
  free(distance);
}

void CBasisFunctions::process(std::vector<float> input)
{
  u32 j, i;

  this->input = input;
  for (j = 0; j < this->functions_count; j++)
  {
    float sum = 0.0;
    for (i = 0; i < this->dimension; i++)
      sum+= (this->input[i] - a[j][i])*(this->input[i] - a[j][i]);

    distance[j] = sum;
    output[j] = exp(-sum*b[j]);
  }
}

void CBasisFunctions::process_linear_combination(std::vector<float> input)
{
  process(input);

  u32 j;
  linear_combination = 0.0;
  for (j = 0; j < this->functions_count; j++)
    linear_combination+= w[j]*output[j];
}

void CBasisFunctions::learn_linear_combination(float required_value, float learning_rate, bool learn_basis)
{
  float error = required_value - linear_combination;

  u32 i, j;

  for (j = 0; j < this->functions_count; j++)
    w[j]+= learning_rate*output[j]*error;

  for (j = 0; j < this->functions_count; j++)
  {
    if (w[j] > w_range)
      w[j] = w_range;

    if (w[j] < -w_range)
      w[j] = -w_range;
  }


  if (learn_basis == false)
    return;

  u32 dist_min_idx = 0;
  for (j = 0; j < this->functions_count; j++)
    if (distance[j] < distance[dist_min_idx])
        dist_min_idx = j;

  float lc = learning_rate*(0.1 + 90.0*abs_(required_value));
  for (i = 0; i < this->dimension; i++)
    a[dist_min_idx][i] = (1.0 - lc)*a[dist_min_idx][i] + lc*input[i];

  for (j = 0; j < this->functions_count; j++)
  {
    b[j]+= -0.1*error*w[j];

    if (b[j] < 0.0)
      b[j] = 0.0;

    if (b[j] > this->b_range)
      b[j] = this->b_range;
  }
}

std::vector<float> CBasisFunctions::get()
{
  return output;
}

float CBasisFunctions::get_linear_combination()
{
  return linear_combination;
}

void CBasisFunctions::save(u32 name_idx)
{
  CLog log((char*)"func.log", this->dimension + 1);

  u32 i, j;
  for (j = 0; j < this->functions_count; j++)
  {
    u32 ptr = 0;
    for (i = 0; i < this->dimension; i++)
      log.add(ptr++, a[j][i]);

    log.add(ptr++, w[j]);
  }

  log.save();
}

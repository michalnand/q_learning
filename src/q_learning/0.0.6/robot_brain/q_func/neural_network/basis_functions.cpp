#include "basis_functions.h"

/*
private:

    float **a, **b;
    std::vector<float> output;
*/

CBasisFunctions::CBasisFunctions(u32 count, u32 dimension, float a_range, float b_range, float w_range)
{
  this->functions_count = count;
  this->dimension = dimension;

  this->a_range = a_range;
  this->b_range = b_range;
  this->w_range = w_range;

  u32 j, i;

  a = (float**)malloc(this->functions_count*sizeof(float*));
  b = (float**)malloc(this->functions_count*sizeof(float*));

  for (j = 0; j < this->functions_count; j++)
  {
    a[j] = (float*)malloc(this->dimension*sizeof(float));
    b[j] = (float*)malloc(this->dimension*sizeof(float));

    for (i = 0; i < this->dimension; i++)
    {
      a[j][i] = this->a_range*rnd_();  //in range -a_range..a_range
      b[j][i] = abs_(this->b_range*rnd_());  //in range 0..b_range
    }
  }

  for (j = 0; j < this->functions_count; j++)
    output.push_back(0.0);

  w = (float*)malloc(this->functions_count*sizeof(float));
  for (j = 0; j < this->functions_count; j++)
    w[j] = 0.1*rnd_()*w_range;

  linear_combination = 0.0;
}

CBasisFunctions::~CBasisFunctions()
{
  u32 j;
  for (j = 0; j < this->functions_count; j++)
  {
    free(a[j]);
    free(b[j]);
  }

  free(a);
  free(b);
  free(w);
}

void CBasisFunctions::process(std::vector<float> input)
{
  u32 j, i;
  for (j = 0; j < this->functions_count; j++)
  {
    float sum = 0.0;
    for (i = 0; i < this->dimension; i++)
      sum+= (input[i] - a[j][i])*(input[i] - a[j][i])*b[j][i];

    output[j] = exp(-sum);
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

void CBasisFunctions::learn_linear_combination(float required_value, float learning_rate)
{
  float error = required_value - linear_combination;

  u32 j;
  for (j = 0; j < this->functions_count; j++)
    w[j]+= learning_rate*error*output[j];

  for (j = 0; j < this->functions_count; j++)
  {
    if (w[j] > w_range)
      w[j] = w_range;

    if (w[j] < -w_range)
      w[j] = -w_range;
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

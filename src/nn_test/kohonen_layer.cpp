#include "kohonen_layer.h"

CKohonenLayer::CKohonenLayer(struct KNNLayerInitStructure nn_init)
{
  this->nn_init = nn_init;
  w = NULL;

  init();
}


CKohonenLayer::~CKohonenLayer()
{
  uninit();
}

void CKohonenLayer::init()
{
  uninit();

  w = (float**)malloc(nn_init.neurons_count*sizeof(float*));

  u32 i, j;

  for (j = 0; j < nn_init.neurons_count; j++)
  {
    w[j] = (float*)malloc(nn_init.inputs_count*sizeof(float));
    for (i = 0; i < nn_init.inputs_count; i++)
      w[j][i] = rnd()*nn_init.weight_range;
  }

  for (i = 0; i < nn_init.inputs_count; i++)
    input.push_back(0.0);

  for (i = 0; i < nn_init.neurons_count; i++)
    output.push_back(0.0);

  min_dist = this->input.size()*this->input.size()*1000.0;
  winning_neuron = 0;
}

void CKohonenLayer::uninit()
{
  u32 j;

  if (w != NULL)
  {
    for (j = 0; j < nn_init.neurons_count; j++)
      free(w[j]);

    free(w);

    w = NULL;
  }

  input.clear();
  output.clear();
}

std::vector<float> CKohonenLayer::get_output()
{
    return output;
}

void CKohonenLayer::process(std::vector<float> input)
{
  this->input = input;

  min_dist = this->input.size()*this->input.size()*1000.0;
  winning_neuron = 0;

  u32 j, i;

  for (j = 0; j < nn_init.neurons_count; j++)
  {
    float sum = 0.0;
    for (i = 0; i < nn_init.inputs_count; i++)
      sum+= abs_(w[j][i] - input[i]);

    sum = sum/(2.0*nn_init.inputs_count);

    if (sum < min_dist)
    {
      min_dist = sum;
      winning_neuron = j;
    }

    output[j] = 2.0*(sum - 0.5);
  }

}

void CKohonenLayer::learn()
{
  u32 i, j;
  for (j = 0; j < nn_init.neurons_count; j++)
  {
    float a = 1.0;
    float k = nn_init.learning_constant;

    if (j != winning_neuron)
    {
      a = -1.0;
      k = k/10.0;
    }

    for (i = 0; i < nn_init.inputs_count; i++)
      w[j][i] = (1.0 - k)*w[j][i] + k*a*input[i];
  }
}

void CKohonenLayer::save(char *file_name)
{

}

void CKohonenLayer::load(char *file_name)
{

}

float CKohonenLayer::rnd()
{
    return ((rand()%2000000) - 1000000)/1000000.0;
}

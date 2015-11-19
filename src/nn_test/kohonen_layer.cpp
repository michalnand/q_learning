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


  for (j = 0; j < nn_init.neurons_count; j++)
  {
    std::vector<float> tmp;
    for (i = 0; i < nn_init.outputs_count; i++)
      tmp.push_back(0.0);

    nn_output.push_back(tmp);
  }
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

//return distances output from each neuron - depends on prcess normalise_output value
//high output[i] value means higher match
std::vector<float> CKohonenLayer::get_output()
{
    return output;
}

std::vector<std::vector<float>>* CKohonenLayer::get_nn_output()
{
    return &nn_output;
}

u32 CKohonenLayer::get_output_winning_neuron_idx()
{
    return winning_neuron;
}

float CKohonenLayer::get_output_winning_distance()
{
    return min_dist;
}

float** CKohonenLayer::get_weights()
{
    return w;
}

void CKohonenLayer::process(std::vector<float> input, bool normalise_output)
{
  this->input = input;

  min_dist = this->input.size()*10.0;
  winning_neuron = 0;

  u32 j, i;

  for (j = 0; j < nn_init.neurons_count; j++)
  {
    float sum = 0.0;
    for (i = 0; i < nn_init.inputs_count; i++)
      sum+= abs_(w[j][i] - input[i]);

    //normalise sum value
    sum = sum/nn_init.inputs_count;

    //limit output value
    if (sum > nn_init.output_limit)
        sum = nn_init.output_limit;

    //find winning neuron
    if (sum < min_dist)
    {
      min_dist = sum;
      winning_neuron = j;
    }

    //reverse value - higher output = closer match to pattern
    sum = nn_init.output_limit - sum;
    output[j] = sum;
  }

  //normalise output into <0..1> range
  if (normalise_output)
  {
      float max = -1000.0;
      float min = -max;

      for (j = 0; j < nn_init.neurons_count; j++)
      {
        if (output[j] > max)
            max = output[j];

        if (output[j] < min)
            min = output[j];
      }

      if (max == min)
      {
          max+= max/100000.0;
      }

      float k = 1.0/(max - min);
      float q = 1.0 - k*max;

      for (j = 0; j < nn_init.neurons_count; j++)
        output[j] = k*output[j] + q;
  }

}

void CKohonenLayer::learn(std::vector<float> *required_output)
{
  u32 i, j;
  for (j = 0; j < nn_init.neurons_count; j++)
  {
    float k = 0.0;
    float a = 1.0;

    if (j != winning_neuron)
    {
        float tmp = 1.0 - output[j];
        k = nn_init.learning_constant * 1.0/(100.0 + tmp);
        a = 1.0;
    }
    else
    {
        k = nn_init.learning_constant;
        a = 1.0;
    }

    for (i = 0; i < nn_init.inputs_count; i++)
      w[j][i] = (1.0 - k)*w[j][i] + k*a*input[i];
  }

  if (required_output != NULL)
  {
    if (nn_output[winning_neuron].size() == required_output->size())
    {
      float k  = nn_init.learning_constant;
      for (i = 0; i < nn_output[winning_neuron].size(); i++)
        nn_output[winning_neuron][i] =
          (1.0 - k)*nn_output[winning_neuron][i] + k*(*required_output)[i];
    }
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

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
    distances_output.push_back(0.0);

  min_dist = this->input.size()*this->input.size()*2*10.0;
  winning_neuron = 0;

  for (j = 0; j < nn_init.neurons_count; j++)
  {
    std::vector<float> tmp;
    for (i = 0; i < nn_init.outputs_count; i++)
      tmp.push_back(0.0);

    learned_output.push_back(tmp);
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
  distances_output.clear();
}

//return distances output from each neuron - depends on process normalise_output value
//high output[i] value means higher match
std::vector<float> CKohonenLayer::get_distances_output()
{
    return distances_output;
}


std::vector<float> CKohonenLayer::get_learned_output()
{
    return learned_output[winning_neuron];
}


std::vector<std::vector<float>> CKohonenLayer::get_learned_output_all()
{
    return learned_output;
}

u32 CKohonenLayer::get_winning_neuron_idx()
{
    return winning_neuron;
}

float CKohonenLayer::get_winning_distance()
{
    return min_dist;
}

float** CKohonenLayer::get_weights()
{
    return w;
}

void CKohonenLayer::process(std::vector<float> input)
{
  this->input = input;

  min_dist = this->input.size()*this->input.size()*10.0;
  winning_neuron = 0;

  u32 j, i;

  for (j = 0; j < nn_init.neurons_count; j++)
  {
    float sum = 0.0;
    for (i = 0; i < nn_init.inputs_count; i++)
      sum+= 0.5*abs_(w[j][i] - input[i]);

    //normalise distance value
    sum = sum/nn_init.inputs_count;

    //limit output value
    if (sum > 1.0)
        sum = 1.0;

    //find winning neuron
    if (sum < min_dist)
    {
      min_dist = sum;
      winning_neuron = j;
    }

    //use exponential distribution,
    //and reverse value - higher output = closer match to pattern
    float a = 100.0; //1000000000.0;
    sum = pow(a, 1.0 - sum)/a;
    distances_output[j] = 1.0 - sum;
  }
}

void CKohonenLayer::learn(std::vector<float> *required_output)
{
  u32 i, j;

  for (j = 0; j < nn_init.neurons_count; j++)
  {
    float k = 1.0;

    if (j == winning_neuron)
        k = nn_init.learning_constant;
    else
        k = 1.0;

    for (i = 0; i < nn_init.inputs_count; i++)
      w[j][i] = k*w[j][i] + (1.0 - k)*input[i];
  }

  if (required_output != NULL)
  {
    if (learned_output[winning_neuron].size() == required_output->size())
    {
      float k  = nn_init.learning_constant;
      for (i = 0; i < learned_output[winning_neuron].size(); i++)
        learned_output[winning_neuron][i] =
          k*learned_output[winning_neuron][i] + (1.0 - k)*(*required_output)[i];
    }
  }
}

void CKohonenLayer::learn_single_output(float required_value, u32 idx)
{
  float k  = nn_init.learning_constant;
  learned_output[winning_neuron][idx] =
      k*learned_output[winning_neuron][idx] + (1.0 - k)*required_value;
}

void CKohonenLayer::save(char *file_name)
{
  FILE *f;
  f = fopen(file_name,"w");

  u32 i, j;
  for (j = 0; j < nn_init.neurons_count; j++)
  {
    for (i = 0; i < nn_init.inputs_count; i++)
      fprintf(f, "%f ", w[j][i]);
    fprintf(f,"\n");
  }

  fclose(f);


  CLog log((char*)"test.txt", 4 + learned_output[winning_neuron].size());

  float x, y, dt = 1.0/100.0;
  for (y = -1.0; y <= 1.0; y+= dt)
    for (x = -1.0; x <= 1.0; x+= dt)
    {
      std::vector<float> input;
      input.push_back(x);
      input.push_back(y);

      process(input);

      log.add(0, x);
      log.add(1, y);
      log.add(2, winning_neuron);
      log.add(3, distances_output[winning_neuron]);

      for (i = 0; i < learned_output[winning_neuron].size(); i++)
        log.add(4+i, learned_output[winning_neuron][i]);
    }

  log.save();
}

void CKohonenLayer::load(char *file_name)
{

}

float CKohonenLayer::rnd()
{
    return ((rand()%20000000) - 10000000)/10000000.0;
}

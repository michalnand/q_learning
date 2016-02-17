#include "echo_state_layer.h"

CEchoStateLayer::CEchoStateLayer(struct EchoStateLayerInitStructure nn_init)
{
  this->nn_init = nn_init;
  w = NULL;

  init();
}


CEchoStateLayer::~CEchoStateLayer()
{
  uninit();
}

void CEchoStateLayer::init()
{
  uninit();

  w = (float**)malloc(nn_init.neurons_count*sizeof(float*));

  w_i_size = nn_init.neurons_count + nn_init.inputs_count;

  u32 i, j;


  for (j = 0; j < nn_init.neurons_count; j++)
  {
      w[j] = (float*)malloc(w_i_size*sizeof(float));
      for (i = 0; i < w_i_size; i++)
      {
          //non zero weights with connection_density probability
          if (abs_(rnd()) < nn_init.connection_densisty)
            w[j][i] = rnd()*nn_init.weight_range;
          else
            w[j][i] = 0.0;
      }
  }

  for (i = 0; i < w_i_size; i++)
    input.push_back(0.0);

  for (i = 0; i < nn_init.neurons_count; i++)
    output.push_back(0.0);
}

void CEchoStateLayer::uninit()
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


void CEchoStateLayer::reset(bool random_state)
{
    u32  i;

    if (random_state == true)
    {
        for (i = 0; i < nn_init.neurons_count; i++)
          output[i] = rnd();
    }
    else
    {
        for (i = 0; i < nn_init.neurons_count; i++)
          output[i] = 0.0;
    }
}

std::vector<float> CEchoStateLayer::get_output()
{
    return output;
}

void CEchoStateLayer::process(std::vector<float> input)
{
    u32 j, i;

    u32 ptr = 0;

    for (i = 0; i < input.size(); i++)
        this->input[ptr++] = input[i];

    for (i = 0; i < output.size(); i++)
        this->input[ptr++] = output[i];

    for (j = 0; j < nn_init.neurons_count; j++)
    {
        float sum = 0.0;
        for (i = 0; i < w_i_size; i++)
            sum+= w[j][i]*this->input[i];

        output[j] = cos(sum);
    }
}

void CEchoStateLayer::learn()
{
    // we dont know how to learn this "structure"
}

void CEchoStateLayer::save(char *file_name)
{

}

void CEchoStateLayer::load(char *file_name)
{

}

float CEchoStateLayer::rnd()
{
    return ((rand()%2000000) - 1000000)/1000000.0;
}

#include "nn_layer.h"

CNNLayer::CNNLayer(struct NNLayerInitStructure nn_init)
{
    this->nn_init = nn_init;
    w = NULL;

    init();
}

CNNLayer::~CNNLayer()
{
    uninit();
}

void CNNLayer::init()
{
    uninit();

    u32 i, j;

    synapses_count = 0;

    w = (float**)malloc(nn_init.neurons_count*sizeof(float*));

    if (
            (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_LINEAR) ||
            (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_TANH) ||
            (nn_init.neuron_type ==  NN_LAYER_NEURON_TYPE_RECTIFIER)
        )
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            w[j] = (float*)malloc(nn_init.inputs_count*sizeof(float));

            for (i = 0; i < nn_init.inputs_count; i++)
                w[j][i] = rnd()*nn_init.weight_range*nn_init.init_weight_range;

            synapses_count = nn_init.inputs_count;
        }
    }


    if (
            (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_INTERSYNAPTICS)
        )
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            u32 v_count = ceil( (nn_init.inputs_count*nn_init.inputs_count + nn_init.inputs_count)/2 );
            u32 tmp = nn_init.inputs_count + v_count;

            w[j] = (float*)malloc(tmp*sizeof(float));

            u32 ptr = 0;
            for (i = 0; i < nn_init.inputs_count; i++)
                w[j][ptr++] = rnd()*nn_init.weight_range*nn_init.init_weight_range;

            for (i = 0; i < v_count; i++)
                w[j][ptr++] = 0.1*rnd()*nn_init.weight_range*nn_init.init_weight_range;

            synapses_count = tmp;
        }
    }

    for (j = 0; j < nn_init.inputs_count; j++)
        input.push_back(0.0);

    for (j = 0; j < nn_init.inputs_count; j++)
        error.push_back(0.0);

    for (j = 0; j < nn_init.neurons_count; j++)
        output.push_back(0.0);
}

void CNNLayer::uninit()
{
    u32 j;

    if (w != NULL)
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            if (w[j] != NULL)
            {
                free(w[j]);
                w[j] = NULL;
            }
        }

        free(w);
        w = NULL;
    }


    input.clear();
    error.clear();
    output.clear();
}

std::vector<float> CNNLayer::get_output()
{
    return output;
}

std::vector<float> CNNLayer::get_error()
{
    return error;
}

void CNNLayer::process(std::vector<float> input)
{
    u32 j, i, k;


    this->input = input;

    if (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_LINEAR)
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            float sum = 0.0;

            for (i = 0; i < nn_init.inputs_count; i++)
                sum+= w[j][i]*this->input[i];

            output[j] = sum;
        }
    }

    if (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_RECTIFIER)
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            float sum = 0.0;

            for (i = 0; i < nn_init.inputs_count; i++)
                sum+= w[j][i]*this->input[i];

            if (sum < 0.0)
              output[j] = 0.0;
            else
              output[j] = sum;
        }
    }

    if (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_TANH)
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            float sum = 0.0;

            for (i = 0; i < nn_init.inputs_count; i++)
                sum+= w[j][i]*this->input[i];

            output[j] = tanh(sum);
        }
    }

    if (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_INTERSYNAPTICS)
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            float sum = 0.0;

            u32 ptr = 0;
            for (i = 0; i < nn_init.inputs_count; i++)
            {
                sum+= w[j][ptr]*this->input[i];

                ptr++;

                for (k = i; k < nn_init.inputs_count; k++)
                    sum+= w[j][ptr++]*this->input[i]*this->input[k];
            }

            output[j] = tanh(sum);
        }
    }

    for (j = 0; j < nn_init.neurons_count; j++)
    {
        if (output[j] > nn_init.output_limit)
            output[j] = nn_init.output_limit;
        if (output[j] < -nn_init.output_limit)
            output[j] = -nn_init.output_limit;
    }
}

void CNNLayer::learn(std::vector<float> error)
{
    u32 j, i, k;

    for (j = 0; j < nn_init.inputs_count; j++)
        this->error[j] = 0.0;

    if (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_LINEAR)
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            for (i = 0; i < nn_init.inputs_count; i++)
            {
                w[j][i]+= nn_init.learning_constant*error[j]*input[i];
                this->error[i]+= w[j][i]*error[j];
            }
        }
    }


    if (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_RECTIFIER)
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            for (i = 0; i < nn_init.inputs_count; i++)
            {
                w[j][i]+= nn_init.learning_constant*error[j]*input[i];
                this->error[i]+= w[j][i]*error[j];
            }
        }
    }

    if (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_TANH)
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            for (i = 0; i < nn_init.inputs_count; i++)
            {
                w[j][i]+= nn_init.learning_constant*error[j]*input[i]; //*output[j]*(1.0 - output[j]);
                this->error[i]+= w[j][i]*error[j]*output[j]*(1.0 - output[j]);
            }
        }
    }


    if (nn_init.neuron_type == NN_LAYER_NEURON_TYPE_INTERSYNAPTICS)
    {
        for (j = 0; j < nn_init.neurons_count; j++)
        {
            u32 ptr = 0;
            for (i = 0; i < nn_init.inputs_count; i++)
            {
                w[j][ptr]+= nn_init.learning_constant*error[j]*input[i]; //*output[j]*(1.0 - output[j]);

                this->error[i]+= w[j][ptr]*error[j]*output[j]*(1.0 - output[j]);

                ptr++;

                for (k = i; k < nn_init.inputs_count; k++)
                {
                    w[j][ptr]+= nn_init.learning_constant*error[j]*input[i]*input[k];
                    this->error[i]+= w[j][ptr]*error[j]*output[j]*(1.0 - output[j])*0.1;
                    ptr++;
                }
            }
        }
    }

    for (j = 0; j < nn_init.neurons_count; j++)
        for (i = 0; i < nn_init.inputs_count; i++)
        {
            if (w[j][i] > nn_init.weight_range)
                w[j][i] = nn_init.weight_range;

            if (w[j][i] < -nn_init.weight_range)
                w[j][i] = -nn_init.weight_range;
        }
}

void CNNLayer::save(char *file_name)
{
    FILE *f;
    u32 j, i;

    f = fopen(file_name, "w");

    fprintf(f, "%u %u %u %u\n", nn_init.neurons_count, nn_init.inputs_count, nn_init.order, nn_init.neuron_type);
    fprintf(f, "%f %f %f %f\n", nn_init.weight_range, nn_init.init_weight_range, nn_init.learning_constant, nn_init.output_limit);

    for (j = 0; j < nn_init.neurons_count; j++)
    {
        for (i = 0; i < nn_init.inputs_count; i++)
            fprintf(f, "%f ", w[j][i]);
        fprintf(f, "\n");
    }

    fclose(f);
}

void CNNLayer::load(char *file_name)
{
    FILE *f;
    u32 j, i;

    f = fopen(file_name, "r");

    u32 res;

    res = fscanf(f, "%u %u %u %u\n", &nn_init.neurons_count, &nn_init.inputs_count, &nn_init.order, &nn_init.neuron_type);
    res = fscanf(f, "%f %f %f %f\n", &nn_init.weight_range, &nn_init.init_weight_range, &nn_init.learning_constant, &nn_init.output_limit);

    init();

    for (j = 0; j < nn_init.neurons_count; j++)
    {
        for (i = 0; i < nn_init.inputs_count; i++)
            res = fscanf(f, "%f ", &w[j][i]);
    }

    fclose(f);
}

float CNNLayer::rnd()
{
	return ((rand()%2000000) - 1000000)/1000000.0;
}

float** CNNLayer::get_w()
{
  return w;
}

void CNNLayer::set_w(float **w_set)
{
  u32 i, j;
  for (j = 0; j < nn_init.neurons_count; j++)
      for (i = 0; i < synapses_count; i++)
        w[j][i] = w_set[j][i];
}

void CNNLayer::noise_w(float level)
{
  u32 i, j;
  for (j = 0; j < nn_init.neurons_count; j++)
      for (i = 0; i < synapses_count; i++)
        w[j][i] = (1.0 - level)*w[j][i] + level*nn_init.weight_range;
}

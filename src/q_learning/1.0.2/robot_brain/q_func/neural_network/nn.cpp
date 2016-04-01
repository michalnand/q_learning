#include "nn.h"

CNN::CNN(struct sNNInitStruct nn_init)
{
    this->nn_init = nn_init;

    layers.clear();

    init();
}

CNN::~CNN()
{
    uninit();
}


void CNN::init()
{
    uninit();

    u32 j;
    for (j = 0; j < nn_init.init_vector[nn_init.init_vector.size()-1]; j++)
    {
        output.push_back(0.0);
        error.push_back(0.0);
    }


    for (j = 0; j < (nn_init.init_vector.size()-1); j++)
    {
        struct NNLayerInitStructure nn_layer_init;

        nn_layer_init.neurons_count = nn_init.init_vector[j + 1];
        nn_layer_init.inputs_count = nn_init.init_vector[j];

        nn_layer_init.order = nn_init.order;
        nn_layer_init.neuron_type = nn_init.neuron_type;

        nn_layer_init.weight_range = nn_init.weight_range;

        if (j != (nn_init.init_vector.size()-2))
          nn_layer_init.init_weight_range = nn_init.init_weight_range;
        else
          nn_layer_init.init_weight_range = 0.0;
          
        nn_layer_init.learning_constant = nn_init.learning_constant;
        nn_layer_init.output_limit = nn_init.output_limit;

        class CNNLayer *nn_layer;
        nn_layer = new CNNLayer(nn_layer_init);
        layers.push_back(nn_layer);

        nn_layer = new CNNLayer(nn_layer_init);
    }
}

void CNN::uninit()
{
    u32 j;

    for (j = 0; j < layers.size(); j++)
    {
        delete layers[j];
        layers[j] = NULL;
    }


    layers.clear();

    output.clear();
    error.clear();
    input.clear();
}



void CNN::process(std::vector<float> input)
{
    u32 j;

    this->input = input;
    for (j = 0; j < layers.size(); j++)
    {
        layers[j]->process(this->input);
        this->input = layers[j]->get_output();
    }

    output = layers[layers.size()-1]->get_output();
}

void CNN::learn(std::vector<float> required_output)
{
    i32 j;
    error.clear();
    for (j = 0; j < output.size(); j++)
        error.push_back(required_output[j] - output[j]);

    for (j = (layers.size()-1); j >= 0; j--)
    {
        layers[j]->learn(error);
        error = layers[j]->get_error();
    }
}

std::vector<float> CNN::get_output()
{
    return output;
}

std::vector<float> CNN::get()
{
    return output;
}

void CNN::save(char *file_name)
{
    u32 j;

    FILE *f;

    f = fopen(file_name, "w");

    fprintf(f, "%u %u\n", nn_init.order, nn_init.neuron_type);
    fprintf(f, "%f %f %f %f\n", nn_init.weight_range, nn_init.init_weight_range, nn_init.learning_constant, nn_init.output_limit);


    for (j = 0; j < nn_init.init_vector.size(); j++)
        fprintf(f, "%u ", nn_init.init_vector[j]);

    fprintf(f, "\n");

    for (j = 0; j < layers.size(); j++)
    {
        char name[1024];
        sprintf(name, "%s_layer_%u", file_name, j);

        fprintf(f,"%s\n", name);

        layers[j]->save(name);
    }

    fclose(f);
}

void CNN::load(char *file_name)
{
    u32 j;

    FILE *f;

    uninit();

    f = fopen(file_name, "r");

    u32 res;

    res = fscanf(f, "%u %u\n", &nn_init.order, &nn_init.neuron_type);
    res = fscanf(f, "%f %f %f %f\n", &nn_init.weight_range, &nn_init.init_weight_range, &nn_init.learning_constant, &nn_init.output_limit);


    for (j = 0; j < layers.size(); j++)
    {
        u32 tmp;
        res = fscanf(f, "%u", &tmp);
        nn_init.init_vector.push_back(res);
    }


    for (j = 0; j < layers.size(); j++)
    {
        char name[1024];
        //sprintf(name, "layer_%u_%s", j, file_name);

        res = fscanf(f,"%s\n", &name);

        layers[j]->load(name);
    }

    fclose(f);
}

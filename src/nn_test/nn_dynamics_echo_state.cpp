#include "nn_dynamics_echo_state.h"

CNNDynamicsEchoState::CNNDynamicsEchoState(struct sNNDynamicsEchoStateInitStruct nn_init)
{
    echo_state_layer = new CEchoStateLayer(nn_init.echo_state_init_struct);

    nn_init.nn_init_struct.inputs_count = nn_init.echo_state_init_struct.neurons_count;

    nn_output_layer = new CNNLayer(nn_init.nn_init_struct);
}

CNNDynamicsEchoState::~CNNDynamicsEchoState()
{
    delete echo_state_layer;
    echo_state_layer = NULL;

    delete nn_output_layer;
    nn_output_layer = NULL;
}

void CNNDynamicsEchoState::init()
{

}

void CNNDynamicsEchoState::uninit()
{

}

void CNNDynamicsEchoState::reset()
{
    echo_state_layer->reset(false);
}

void CNNDynamicsEchoState::process(std::vector<float> input)
{
    echo_state_layer->process(input);

    nn_output_layer->process(echo_state_layer->get_output());

    output = nn_output_layer->get_output();
}

void CNNDynamicsEchoState::learn(std::vector<float> required_output)
{
    std::vector<float> error;

    u32 i;

    for (i = 0; i < output.size(); i++)
        error.push_back(required_output[i] - output[i]);

    nn_output_layer->learn(error);
}

std::vector<float> CNNDynamicsEchoState::get_output()
{
    return output;
}

std::vector<float> CNNDynamicsEchoState::get()
{
    return output;
}

void CNNDynamicsEchoState::save(char *file_name)
{

}

void CNNDynamicsEchoState::load(char *file_name)
{

}

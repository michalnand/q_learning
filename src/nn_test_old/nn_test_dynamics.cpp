#include "nn_test_dynamics.h"


CDynamicsTest::CDynamicsTest()
{
    struct sNNDynamicsEchoStateInitStruct init_struct;

    init_struct.echo_state_init_struct.neurons_count = 128;
    init_struct.echo_state_init_struct.inputs_count = 2;
    init_struct.echo_state_init_struct.weight_range = 4.0;
    init_struct.echo_state_init_struct.connection_densisty = 1.0/100.0;

    init_struct.nn_init_struct.neurons_count = 1;
    init_struct.nn_init_struct.inputs_count = init_struct.echo_state_init_struct.neurons_count;
    init_struct.nn_init_struct.order = 0;
    init_struct.nn_init_struct.neuron_type = NN_LAYER_NEURON_TYPE_LINEAR;

    init_struct.nn_init_struct.weight_range = 4.0;
    init_struct.nn_init_struct.init_weight_range = 0.25*init_struct.nn_init_struct.weight_range;
    init_struct.nn_init_struct.learning_constant = 1.0/100.0;
    init_struct.nn_init_struct.output_limit = 4.0;


    nn_dynamics = new CNNDynamicsEchoState(init_struct);

    u32 i;
    for (i = 0; i < init_struct.echo_state_init_struct.inputs_count; i++)
        input.push_back(0.0);

    for (i = 0; i < init_struct.nn_init_struct.neurons_count; i++)
        output.push_back(0.0);
}

CDynamicsTest::~CDynamicsTest()
{
    delete nn_dynamics;
}

void CDynamicsTest::process_single_test()
{
    u32 k;
    u32 iterations = 100000;

    class CLog *log;
    log = new CLog((char*)"dynamics_results/nn_dynamics_log.txt", 4);

    nn_dynamics->reset();

    float required_output_ = 0.0;
    float filter_coef = 0.9;
    float error = 0.0;

    std::vector<float> required_output;

    required_output.push_back(0.0);

    for (k = 0; k < iterations; k++)
    {
        float tmp = 0.0;
        u32 period = 200;

        float t = k/30.0;

        if ((k%period) < (period/2))
            input[0] = 0.0;
        else
        {
            input[0] = (sin(t*3.7) + 0.7*sin(t) + 0.5867*sin(t + 4534.0435) + 0.7967*sin(t + 534.0435));

            if (k > (0.995*iterations))
                input[0] = 1.0;

        }

        input[1] = 0.0;

        nn_dynamics->process(input);

        output = nn_dynamics->get_output();
        required_output_ = -0.9*required_output_*required_output_*required_output_ + required_output_*filter_coef + (1.0-filter_coef)*(-1.0*input[0]);

        required_output[0] =  2.0*required_output_;

        //error = abs_(required_output[0] - output[0])/(0.0000001 + abs_(required_output[0]));
        error = required_output[0] - output[0];

        if (k > (0.995*iterations))
        {
            log->add(0, input[0]);
            log->add(1, required_output[0]);
            log->add(2, output[0]);
            log->add(3, error);
        }
        else
            nn_dynamics->learn(required_output);

    }

    log->save();
    delete log;
}

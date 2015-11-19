#ifndef _NN_DYNAMICS_ECHO_STATE_H_
#define _NN_DYNAMICS_ECHO_STATE_H_

#include "echo_state_layer.h"

#include "nn_layer.h"

struct sNNDynamicsEchoStateInitStruct
{
    struct EchoStateLayerInitStructure echo_state_init_struct;
    struct NNLayerInitStructure nn_init_struct;
};


class CNNDynamicsEchoState
{
    private:
        std::vector<float> output;

        class CEchoStateLayer *echo_state_layer;
        class CNNLayer *nn_output_layer;

    public:
        CNNDynamicsEchoState(struct sNNDynamicsEchoStateInitStruct nn_init);
        ~CNNDynamicsEchoState();

        void init();
        void uninit();
        void reset();

        void process(std::vector<float> input);
        void learn(std::vector<float> required_output);

        std::vector<float> get_output();
        std::vector<float> get();

        void save(char *file_name);
        void load(char *file_name);
};

#endif

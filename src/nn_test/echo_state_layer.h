#ifndef _ECHO_STATE_LAYER_H_
#define _ECHO_STATE_LAYER_H_

#include "common.h"

struct EchoStateLayerInitStructure
{
    u32 neurons_count, inputs_count;

    float weight_range, connection_densisty;
};

class CEchoStateLayer
{
    private:
            struct EchoStateLayerInitStructure nn_init;
            float **w;

            u32 w_i_size;
            std::vector<float> input, output;

    public:
            CEchoStateLayer(struct EchoStateLayerInitStructure nn_init);
            ~CEchoStateLayer();

            void init();
            void uninit();
            void reset(bool random_state);

            std::vector<float> get_output();

            void process(std::vector<float> input);
            void learn();

            void save(char *file_name);
            void load(char *file_name);

            float rnd();
};

#endif

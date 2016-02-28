#ifndef _NN_LAYER_H_
#define _NN_LAYER_H_

#include "../../../common.h"

#define NN_LAYER_NEURON_TYPE_LINEAR                 (u32)0
#define NN_LAYER_NEURON_TYPE_TANH                   (u32)1
#define NN_LAYER_NEURON_TYPE_INTERSYNAPTICS         (u32)2

#define NN_LAYER_NEURON_TYPE_RECTIFIER               (u32)3

struct NNLayerInitStructure
{
    u32 neurons_count, inputs_count, order, neuron_type;

    float weight_range, init_weight_range, learning_constant, output_limit;
};


class CNNLayer
{
    private:
        u32 synapses_count;
        struct NNLayerInitStructure nn_init;
        float **w;

        std::vector<float> input, output, error;

    public:
        CNNLayer(struct NNLayerInitStructure nn_init);
        ~CNNLayer();

        void init();
        void uninit();

        std::vector<float> get_output();
        std::vector<float> get_error();

        void process(std::vector<float> input);
        void learn(std::vector<float> error);

        void save(char *file_name);
        void load(char *file_name);

        float rnd();

        float **get_w();
        void set_w(float **w_set);
        void noise_w(float level);
};

#endif

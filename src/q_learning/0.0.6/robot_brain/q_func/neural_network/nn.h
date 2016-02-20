#ifndef _NN_H_
#define _NN_H_

#include "nn_layer.h"

struct sNNInitStruct
{
    std::vector<u32> init_vector;

    u32 order, neuron_type;
    float weight_range, init_weight_range, learning_constant, output_limit;
};

class CNN
{
    private:
        std::vector<float> error, output, input;

        struct sNNInitStruct nn_init;

        std::vector<CNNLayer*> layers;

    public:
        CNN(struct sNNInitStruct nn_init);
        ~CNN();

        void init();
        void uninit();

        void process_testing(std::vector<float> input);
        void process(std::vector<float> input);
        void learn(std::vector<float> required_output);

        std::vector<float> get_output();
        std::vector<float> get();

        void save(char *file_name);
        void load(char *file_name);
};

#endif

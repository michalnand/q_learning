#ifndef _KOHONEN_LAYER_H_
#define _KOHONEN_LAYER_H_

#include "common.h"

struct KNNLayerInitStructure
{
    u32 neurons_count, inputs_count;

    float weight_range, learning_constant, output_limit;
};

class CKohonenLayer
{
    private:
            struct KNNLayerInitStructure nn_init;
            float **w;

            std::vector<float> input, output;

            float min_dist;
            u32 winning_neuron;


    public:
            CKohonenLayer(struct KNNLayerInitStructure nn_init);
            ~CKohonenLayer();

            void init();
            void uninit();

            std::vector<float> get_output();
            u32 get_output_winning_neuron_idx();
            float get_output_winning_distance();
            
            float** get_weights();

            void process(std::vector<float> input);
            void learn();

            void save(char *file_name);
            void load(char *file_name);

            float rnd();
};

#endif

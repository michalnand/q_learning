#ifndef _KOHONEN_LAYER_H_
#define _KOHONEN_LAYER_H_

#include "../../../common.h"

struct KNNLayerInitStructure
{
    u32 neurons_count, inputs_count, outputs_count;

    float weight_range, learning_constant, output_limit;
};

class CKohonenLayer
{
    private:
            struct KNNLayerInitStructure nn_init;
            float **w;

            std::vector<float> input, distances_output;

            float min_dist;
            u32 winning_neuron;

            std::vector<std::vector<float>> learned_output;


    public:
            CKohonenLayer(struct KNNLayerInitStructure nn_init);
            ~CKohonenLayer();

            void init();
            void uninit();


            std::vector<float> get_distances_output();
            std::vector<float> get_learned_output();
            std::vector<std::vector<float>> get_learned_output_all();

            u32 get_winning_neuron_idx();
            float get_winning_distance();

            float** get_weights();

            void process(std::vector<float> input);
            void learn(std::vector<float> *required_output  = NULL);
            void learn_single_output(float required_value, u32 idx);


            void save(char *file_name);
            void load(char *file_name);

            float rnd();
};

#endif

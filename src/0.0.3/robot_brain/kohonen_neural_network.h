#ifndef _KOHONEN_NEURAL_NETWORK_H_
#define _KOHONEN_NEURAL_NETWORK_H_

#include "../common.h"

struct sKohonenNeuralNetworkInitStructure
{
    u32 input_size, output_size;

    float weight_range, learning_constant, weight_range_init;
};

class CKohonenNeuralNetwork
{
	private:
        struct sKohonenNeuralNetworkInitStructure nn_init;

		std::vector<float> input;
		std::vector<float> output;

        float **w;

        u32 winning_neuron;

	public:

        CKohonenNeuralNetwork(struct sKohonenNeuralNetworkInitStructure nn_init_structure);
		~CKohonenNeuralNetwork();

		void process(std::vector<float> input);
        void process_without_learn(std::vector<float> input);

		std::vector<float> get();

		float** get_weights();
		float** get_dweights();

		void set_weights(float **w);
		void set_weight(u32 i, u32 j, float value);

		void merge_weights(float **w, float weight = 0.5);
		void merge_dweights(float **dw);

	    void set_learning_constant(float learning_constant);

        u32 get_winning_neuron();
 
	private:

		float rnd();
};


#endif

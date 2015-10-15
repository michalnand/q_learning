#ifndef _NEURAL_NETWORK_ECHO_STATE_H_
#define _NEURAL_NETWORK_ECHO_STATE_H_

#include "../common.h"


#define NEURON_TYPE_COMMON	((u32)1)
#define NEURON_TYPE_MIXED	((u32)2)

#define NEURAL_NETWORK_MAGIC 	(u32)0xABCD0003


struct sEchoStateNeuralNetworkInitStructure
{
	u32 init_vector_size;
	u32 *init_vector;

	float weight_range, learning_constant, weight_range_init;

	u32 order, neuron_type;
};

struct sEchoStateNeuralNetwork
{
	u32 layers_count, order;
	u32 *size_input, *size_input_;
	u32 *size_output;

	float learning_constant;
	float weight_range;

	u32 neuron_type;

	float ***w;
	float ***dw;
	float **output;
	float **error;

	float **input;
	float **input_;
};

void EchoStateNeuralNetworkInitStructure_init(
										struct sEchoStateNeuralNetworkInitStructure *nn_init_structure,
										u32 init_vector_size,
										float weight_range,
										u32 order,
										u32 neuron_type,
										float learning_constant,
										float weight_range_init
									);

void EchoStateNeuralNetworkInitStructure_uninit(struct sEchoStateNeuralNetworkInitStructure *nn_init_structure);





class CEchoStateNeuralNetwork
{
	private:
		struct sEchoStateNeuralNetworkInitStructure nn_init_structure;
		struct sEchoStateNeuralNetwork nn;
		float nn_output_limit;


		std::vector<float> input;
		std::vector<float> output;
	public:

		CEchoStateNeuralNetwork(struct sEchoStateNeuralNetworkInitStructure nn_init_structure);
		~CEchoStateNeuralNetwork();

		void process(std::vector<float> input);

		std::vector<float> get();

		float*** get_weights();
		float*** get_dweights();

		void set_weights(float ***w);
		void set_weight(u32 i, u32 j, u32 k, float value);

		void merge_weights(float ***w, float weight = 0.5);
		void merge_dweights(float ***dw);

		void learn(std::vector<float> required_output);

		void set_learning_constant(float learning_constant);

		i32 save_weights(char *file_name);
		i32 load_weights(char *file_name);

	private:

		float rnd();
};

#endif

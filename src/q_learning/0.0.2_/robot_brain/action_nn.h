#ifndef _ACTION_NN_H_
#define _ACTION_NN_H_


struct sActionNN
{
	std::vector<float> 				input;
	std::vector<std::vector<float>> action_output;
};


class CActionNN
{
	private:
		u32 action_id;
		std::vector<struct sActionNN> actions;


	public:
		CActionNN(u32 actions_count, u32 input_size, u32 action_size);
		~CActionNN();


	struct sActionNN process(std::vector<float> input);
	struct sActionNN get();
};


#endif
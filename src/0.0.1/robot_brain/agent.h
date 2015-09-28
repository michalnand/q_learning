#ifndef _AGENT_H_
#define _AGENT_H_

#include "action.h"
#include "q_learning.h"

#define AGENT_TYPE_NULL			(u32)0
#define AGENT_TYPE_COMMON		(u32)1
#define AGENT_TYPE_GREEDY		(u32)2
#define AGENT_TYPE_EXPLORER		(u32)3

struct sAgent
{
	u32 id, type;
	u32 outputs_count, inputs_count;
	u32 actions_per_state;


	float score, reward;

	std::vector<float> state;

	struct sAction output_action;

	float state_density;
};


void s_agent_init(
					struct sAgent *agent,

					u32 id, u32 type,
					u32 outputs_count, u32 inputs_count,
					u32 actions_per_state,
					float state_density
				);

class CAgent
{
	private:
		struct sAgent agent;

		class CAgent 		*collective_agent;
		class CAction 		*actions;

		#ifdef Q_LEARNING_NN
		class CQLearningNN 	*q_learning;
		#else
		class CQLearning 	*q_learning;
		#endif

	public:

		CAgent(struct sAgent agent_init, class CAgent *collective_agent = NULL);
		CAgent(char *file_name, class CAgent *collective_agent = NULL);
		~CAgent();

		void save(char *file_name);

		//process input and learn using reward
		void process(struct sAgent *agent_);
		void reset(struct sAgent *agent_);

		//get output
		struct sAction get_action();

		//merging functions
		#ifdef Q_LEARNING_NN
		CQLearningNN* get_brain();
		#else
		CQLearning* get_brain();
		#endif

		void merge();
		void print(std::vector<float> subspace);
};


#endif

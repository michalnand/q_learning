#ifndef _Q_LEARNING_H_
#define _Q_LEARNING_H_

#include "action.h"
#include "q_func.h"
#include "q_func_nn.h"
#include "q_func_knn.h"

//#define Q_FUNC_NN  1
#define Q_FUNC_KNN  1

struct sQRes
{
	std::vector<float> state;
	struct sAction action;

	float q_max, q;
	u32 action_id;

	float reward;
};

class CQLearning
{
	private:
		u32 state_dimensions;

		float states_density;
		float gamma, alpha;

		struct sQRes q_res, q_res_prev;

		class CAction *actions;

		#ifdef Q_FUNC_NN
		class CQFuncNN  *q_func;
		#else

		#ifdef Q_FUNC_KNN
		class CQFuncKNN  *q_func;
		#else

		class CQFunc *q_func;

		#endif
		#endif

	public:
		CQLearning(	float states_density,
					u32 state_dimensions,
					u32 actions_per_state,
					float gamma = 0.9, float alpha = 0.0,
					class CAction *actions = NULL);

		~CQLearning();

		void process(std::vector<float> state, float reward, float k = 2.0, float explore_prob = 0.0);

		u32 get_states_count();

		u32 get_state_idx();
		u32 get_output_id();

		float get_max_q(std::vector<float> state);

		float get_min_q(std::vector<float> state);
		u32 get_max_q_id(std::vector<float> state);

		#ifdef Q_FUNC_NN
		class CQFuncNN* get_func();
		#else
		#ifdef Q_FUNC_KNN
		class CQFuncKNN* get_func();
		#else
		class CQFunc* get_func();
		#endif
		#endif

		void merge(CQLearning *q_learning);

	private:
		u32 select_action(std::vector<float> state, float k = 2.0, float explore_prob = 0.0);
		void normalise();
};

#endif

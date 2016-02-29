#ifndef _Q_FUNC_H_
#define _Q_FUNC_H_

#include "../../common.h"

#define Q_FUNC_MAGIC    (u32)0xABCD0001

class CQFunc
{
    private:
        std::vector<u32> init_vector;
        std::vector<float> q_idx_f;
        class CArray *q_values;

        u32 state_dimensions;
        float alpha;

    public:
        CQFunc( u32 state_size, u32 action_size,
                float density, float action_density,
                float alpha = 0.9);
        ~CQFunc();

        void learn_start();
        void learn_finish();

        float get(std::vector<float> state, std::vector<float> action);
        void learn(std::vector<float> state, std::vector<float> action, float required_value);

        i32 save(char *file_name);
        i32 load(char *file_name);


        CArray* get_q_values();
        void merge(class CQFunc *q_func);
};

#endif

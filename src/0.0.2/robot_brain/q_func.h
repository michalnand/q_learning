#ifndef _Q_FUNC_H_
#define _Q_FUNC_H_

#include "../common.h"

#define Q_FUNC_MAGIC    (u32)0xABCD0001

class CQFunc
{
    private:
        std::vector<std::vector<float>> q_values;

        class CAssociativeArray *state_associative_array;
        class CAssociativeArray *action_associative_array;


        u32 state_dimensions;
        float alpha;

    public:
        CQFunc( u32 state_size, u32 action_size,
                float density, float action_density,
                float alpha = 0.9);
        ~CQFunc();

        float get(std::vector<float> state, std::vector<float> action);
        void learn(std::vector<float> state, std::vector<float> action, float required_value);

        i32 save(char *file_name);
        i32 load(char *file_name);


        float get_q_value(u32 j, u32 i);
        void merge(class CQFunc *q_func);
};

#endif

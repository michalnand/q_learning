#ifndef _NN_DYNAMICS_H_
#define _NN_DYNAMICS_H_

#include "nn.h"

struct sNNDynamicsInitStruct
{
    struct sNNInitStruct nn_init;
    u32 order;
};


class CNNDynamics
{
    private:

        std::vector<std::vector<float>> input;
        std::vector<std::vector<float>> output;

        class CNN *nn;

    public:
        CNNDynamics(struct sNNDynamicsInitStruct nn_init);
        ~CNNDynamics();

        void init();
        void uninit();
        void reset();

        void process(std::vector<float> input);
        void learn(std::vector<float> required_output);

        std::vector<float> get_output();
        std::vector<float> get();

        void save(char *file_name);
        void load(char *file_name);
};

#endif

#ifndef _NN_TEST_DYNAMICS_H_
#define _NN_TEST_DYNAMICS_H_

#include "common.h"
#include "nn_dynamics_echo_state.h"

class CDynamicsTest
{
    private:
        class CNNDynamicsEchoState *nn_dynamics;

        std::vector<float> input, output;

    public:
        CDynamicsTest();
        ~CDynamicsTest();

        void process_single_test();
};

#endif

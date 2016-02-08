#ifndef _CONTROLL_TEST_H_
#define _CONTROLL_TEST_H_

#include "../common.h"

#include "input.h"
#include "plant.h"
#include "controller.h"

class CControllTest
{
    private:
        class CInput *input;
        class CPlant *plant;

        class CController *controller;

        std::vector<float> state;

    public:
        CControllTest();
        ~CControllTest();

        void run(i32 iterations);

        std::vector<float> get_state();

    private:
        void process_step(bool explore);

};

#endif

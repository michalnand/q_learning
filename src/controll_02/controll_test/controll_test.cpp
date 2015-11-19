#include "controll_test.h"



CControllTest::CControllTest()
{
    float dt = 1.0/100.0;

    input = new CInput(dt);
    plant = new CPlant(dt);
    controller = new CController(dt);
}

CControllTest::~CControllTest()
{
    delete input;
    delete plant;
    delete controller;
}

void CControllTest::run(i32 iterations)
{
    i32 i = 0;
    if (iterations == -1)
    {
        while (1)
        {
            printf("iteration %i : %f\n", i, controller->get_reward_sum());

            if ((i%10) == 0)
                process_step(false);
            else
                process_step(true);
            i++;
        }
    }
    else
    {
        for (i = 0; i < iterations; i++)
        {
            printf("iteration %i : %f\n", i, controller->get_reward_sum());

            if ((i%10) == 0)
                process_step(false);
            else
                process_step(true);
        }
    }

    process_step(false);
}


void CControllTest::process_step(bool explore)
{
    u32 t;
    CLog *response_log, *q_log;

    if (explore == false)
    {
        response_log = new CLog((char*)"response_log.txt", 5);
        q_log = new CLog((char*)"q_log.txt", 3);
    }

    input->reset();
    plant->reset();
    controller->reset();

    float input_value = 0.0;
    float controller_output = 0.0;
    float plant_value = 0.0;

    /*
    if (explore == false)
    {
        u32 shift = rand()%10000;
        for (t = (0+shift); t < (400+shift); t++)
            input->process();
    }
    */

    for (t = 0; t < 400; t++)
    {
        input->process();
        input_value = input->get();

        controller->process(input_value, plant_value, explore);

        controller_output = controller->get();
        plant->process(controller_output);

        plant_value = plant->get();

        if (explore == false)
        {
            float error = input_value - plant_value;

            response_log->add(0, input_value);
            response_log->add(1, plant_value);
            response_log->add(2, controller_output);
            response_log->add(3, controller->get_reward());
            response_log->add(4, error);

            q_log->add(0, controller->get_q_max());
            q_log->add(1, controller->get_state()[0]);
            q_log->add(2, controller->get_reward());
        }
    }

    if (explore == false)
    {
        response_log->save();
        delete response_log;

        q_log->save();
        delete q_log;

        printf("saved\n" );

        controller->print();
    }
}

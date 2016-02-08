#ifndef _INPUT_H_
#define _INPUT_H_

#include "../common.h"

class CInput
{
    private:
        float dt;
        float time;
        float output;
        u32 time_cnt;

    public:
        CInput(float dt = 1.0/100.0);
        ~CInput();

        void reset();
        void process();

        float get_time();
        float get();
};

#endif

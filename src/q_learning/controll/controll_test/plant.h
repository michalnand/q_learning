#ifndef _PLANT_H_
#define _PLANT_H_

#include "../common.h"

class CPlant
{
    private:
        float dt;
        float time;
        float output;

    public:
        CPlant(float dt = 1.0/100.0);
        ~CPlant();

        void reset();
        void process(float input);

        float get_time();
        float get();
};

#endif

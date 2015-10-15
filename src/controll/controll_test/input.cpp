#include "input.h"


CInput::CInput(float dt)
{
    this->dt = dt;

    reset();
}

CInput::~CInput()
{

}

void CInput::process()
{
    u32 tmp = 400;
    if ((time_cnt%tmp) < (tmp/2))
    {
        output = 0.0;
        output+= 1.0*sin(time + 0.45645);
        output+= 0.6*sin(time*2.35467 + 0.6786756);
        output+= 1.7*sin(time*5.345367);
        output+= 1.9*sin(time*0.79657 + 0.54635);
        output+= sgn(sin(time*3.423432564));
        output/= 5.0;
    }
    else
    {
        output = 0.0;
        output+= sgn(2.0*sin(time*6.423432564));
    }

    time+= dt;
    time_cnt++;

    output = saturate(output, -1.0, 1.0);
}

void CInput::reset()
{
    time = 0.0;
    time_cnt = 0;
    output = 0.0;
}

float CInput::get()
{
    return output;
}

float CInput::get_time()
{
    return time;
}

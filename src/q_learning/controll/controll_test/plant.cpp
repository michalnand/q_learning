#include "plant.h"

CPlant::CPlant(float dt)
{
    time = 0.0;
    output = 0.0;

    this->dt = dt;
}

CPlant::~CPlant()
{

}

void CPlant::reset()
{
    time = 0.0;
    output = 0.0;
}

void CPlant::process(float input)
{
    float a = 0.95;
    float b = 0.3;

    output = a*output + b*input; // - 0.3*tanh(output*1.5);

    output = saturate(output, -2.0, 2.0);
}

float CPlant::get_time()
{
    return time;
}

float CPlant::get()
{
    return output;
}

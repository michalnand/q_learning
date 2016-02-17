#include "vector_func.h"

//return vector size
float vector_size(std::vector<float> v)
{
    u32 j;
    float res = 0.0;
    for (j = 0; j < v.size(); j++)
        res+= v[j]*v[j];

    return sqrt(res);
}

//normalise vector into <a, b> interval
void vector_normalise(std::vector<float> *v, float a, float b)
{
    float max_ = -10000000000.0;
    float min_ = -max_;

    u32 j;

    for (j = 0; j < v->size(); j++)
    {
      if ((*v)[j] > max_)
          max_ = (*v)[j];

      if ((*v)[j] < min_)
          min_ = (*v)[j];
    }

    if (max_ == min_)
    {
        max_+= max_/100000.0;
    }

    float k = (b - a)/(max_ - min_);
    float q = b - k*max_;

    for (j = 0; j < v->size(); j++)
      (*v)[j] = k*(*v)[j] + q;
}

//multiply vector by constant
std::vector<float> vector_(std::vector<float> va, float k)
{
    u32 j;
    std::vector<float> res;

    for (j = 0; j < va.size(); j++)
        res.push_back(va[j]*k);

    return res;
}

//add two vectors
std::vector<float> vector_add(std::vector<float> va, std::vector<float> vb)
{
    u32 j;
    std::vector<float> res;

    for (j = 0; j < va.size(); j++)
        res.push_back(va[j] + vb[j]);

    return res;
}

//substract two vector
std::vector<float> vector_sub(std::vector<float> va, std::vector<float> vb)
{
    u32 j;
    std::vector<float> res;

    for (j = 0; j < va.size(); j++)
        res.push_back(va[j] - vb[j]);

    return res;
}

//weighted mix two vectors res = k*va + (1.0 - k)*vb , where k <0, 1>
std::vector<float> vector_mix(std::vector<float> va, std::vector<float> vb, float k)
{
    u32 j;
    std::vector<float> res;

    if (k > 1.0)
        k = 1.0;
    if (k < 0.0)
        k = 0.0;

    for (j = 0; j < va.size(); j++)
        res.push_back(k*va[j] + (1.0 - k)*vb[j]);

    return res;
}

//scalar multiply two vectors
float vector_multiply(std::vector<float> va, std::vector<float> vb)
{
    u32 j;
    float res = 0.0;

    for (j = 0; j < va.size(); j++)
        res+= va[j]*vb[j];

    return res;
}

//return vectors distance
float vector_dist(std::vector<float> va, std::vector<float> vb)
{
    u32 j;
    float res = 0.0;

    for (j = 0; j < va.size(); j++)
        res+= (va[j] - vb[j])*(va[j] - vb[j]);

    return sqrt(res);
}

void print_vector(std::vector<float> v)
{
    u32 j;
    for (j = 0; j < v.size(); j++)
        printf("%6.3f ", v[j]);
}

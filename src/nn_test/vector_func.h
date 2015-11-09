#ifndef _VECTOR_FUNC_H_
#define _VECTOR_FUNC_H_

#include "common.h"

//return vector size
float vector_size(std::vector<float> v);

//normalise vector into <a, b> interval
void vector_normalise(std::vector<float> *v, float a, float b);

//multiply vector by constant
std::vector<float> vector_(std::vector<float> va, float k);

//add two vectors
std::vector<float> vector_add(std::vector<float> va, std::vector<float> vb);

//substract two vector
std::vector<float> vector_sub(std::vector<float> va, std::vector<float> vb);

//scalar multiply two vectors
float vector_multiply(std::vector<float> va, std::vector<float> vb);

//return vectors distance
float vector_dist(std::vector<float> va, std::vector<float> vb);

//weighted mix two vectors res = k*va + (1.0 - k)*vb , where k <0, 1>
std::vector<float> vector_mix(std::vector<float> va, std::vector<float> vb, float k);

void print_vector(std::vector<float> v);

#endif

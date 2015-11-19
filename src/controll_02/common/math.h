#ifndef _MATH__H_
#define _MATH__H_

#include "int_def.h"
#include <math.h>
#include <vector>

#define PI 3.141592654


struct sVect3D
{
	float x, y, z;
}; 

struct sPoint3D 
{
	float x, y, z;
	float r, g, b;
};

struct sVector
{
    float *points;
    u32 size;
};

float saturate(float value, float min, float max);


float rand_();
float rnd_();

float tan_(float x, float y);
float atan_(float x, float y);

float min(float a, float b);
float max(float a, float b);

float min_array(float *values, u32 size);
float max_array(float *values, u32 size);
float abs_(float a);
float sgn(float a);

float angle_modulo(float angle);
void rotate_point(float *x_res, float *y_res, float x, float y, float angle);

float point_from_line_distance(float px, float py, float x0, float y0, float x1, float y1);
void line_intersection(	float *xr, float *yr,
						float x1, float y1, float x2, float y2,
						float x3, float y3, float x4, float y4 ); 

u32 ray_in_sphere(float xd, float x0, float yd, float y0, float xc, float yc, float r);

float fast_tanh(float x);

float line_circle_intersection(float x1, float y1, float x2, float y2, float xc, float yc, float r);

float vect_distance(float *va, float *vb, u32 size);

void vector_normalise(std::vector<float> *vector);



float map_to_interval(float min_a, float max_a, float min_b, float max_b, float value);
void normalise_mat(std::vector<std::vector<float>> *mat);


void vect_print(std::vector<float> v);

#endif
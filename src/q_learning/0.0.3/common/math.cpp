#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "math.h"


float saturate(float value, float min, float max)
{
    if (value < min)
        value = min;

    if (value > max)
        value = max;

    return value;
}
 
float rand_()
{
    i32 tmp = (rand()%1000000); 
    
    return tmp/1000000.0;
}

float rnd_()
{
    i32 tmp = (rand()%2000000) - 1000000; 
    
    return tmp/1000000.0;
}

float tan_(float x, float y)
{
    float pi = 3.141592654;

    float x_ = x;
    float y_ = y; 

    if (x_ < 0.0)
        x_ = -x_;

    if (y_ < 0.0)
        y_ = -y_;


    if (x_ == 0.0)
    {
        x_ = 0.0000001;
    }

    float res = tan(y_/x_);


    if ((x > 0.0) && (y < 0.0))
        res+= pi;
    
    if ((x < 0.0) && (y < 0.0))
        res+= 2*pi;
    
    if ((x > 0.0) && (y < 0.0))
        res-= pi/4.0;
 
 
    return res;
}


float atan_(float x, float y)
{
    float angle = 0.0;

    if (x >= 0.0)
    {
        if (y >= 0.0)
        {
            if (x == 0.0)
                x = 0.00001;
            angle = PI/2.0 + atan(y/x);
        }
        else
        {
            if (x == 0.0)
                x = 0.00001;
            angle = PI/2.0 + atan(y/x);
        }
    }
    else
    {
        if (y >= 0.0)
        {
            if (x == 0.0)
                x = 0.00001;
            angle = -(PI/2.0 - atan(-y/-x));
        }
        else
        {
            if (x == 0.0)
                x = 0.00001;
            angle = -(PI/2.0 - atan(-y/-x));
        }
    }

    /*
    if (x >= 0.0)
    {
        if (y >= 0.0)
        {
            if (x == 0.0)
                x = 0.00001;
            angle = PI/2.0 + atan(y/x);
        }
        else
        {
            if (x == 0.0)
                x = 0.00001;
        angle = -PI/2.0 + atan(y/x);  
        }
    }
    else
    {
        if (y >= 0.0)
        {
            if (x == 0.0)
                x = 0.00001;
            angle = -PI/2.0 + atan(y/x);
        }
        else
        {
            if (x == 0.0)
                x = 0.00001;
            angle = PI/2.0 + atan(y/x);
        }
    }
    */

    return angle;
}


float min(float a, float b)
{
	if (a < b)
		return a;
	return b;
}
 
float max(float a, float b)
{
	if (a > b)
		return a;
	return b;
}

float min_array(float *values, u32 size)
{
    u32 i;
    float res = values[0];

    for (i = 1; i < size; i++)
        if (values[i] < res)
            res = values[i];

    return res;
}

float max_array(float *values, u32 size)
{
    u32 i;
    float res = values[0];

    for (i = 1; i < size; i++)
        if (values[i] > res)
            res = values[i];

    return res;
}


float abs_(float a)
{
    if (a < 0.0)
        return -a;
    return a;
}

float sgn(float a)
{
    if (a > 0.0)
        return 1.0;

    if (a < 0.0)
        return -1.0;

    return 0.0;
}

float angle_modulo(float angle)
{
    i32 tmp = (10000*angle)/(2.0*PI);

    tmp = tmp%10000;

    angle = 2.0*PI*tmp/10000.0;

    if (angle < 0.0)
        angle = 2.0*PI + angle;

        

    return angle;
}


void rotate_point(float *x_res, float *y_res, float x, float y, float angle)
{
    *x_res = x*cos(angle) - y*sin(angle);
    *y_res = x*sin(angle) + y*cos(angle);
}


float point_from_line_distance(float px, float py, float x0, float y0, float x1, float y1)
{
    float res = 0.0;

    float dx = x1 - x0;
    float dy = y1 - y0;

    res = dy*px - dx*py - x0*y1 + x1*y0;
    if (res < 0.0)
        res = -res;

    res/= sqrt(dx*dx + dy*dy); 

    return res;
}


void line_intersection( float *xr, float *yr,
                        float x1, float y1, float x2, float y2,
                        float x3, float y3, float x4, float y4 )
{ 
    float tmp;
 

    tmp = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);

    *xr = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4))/tmp;

    *yr = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4))/tmp;
}


u32 ray_in_sphere(float xd, float x0, float yd, float y0, float xc, float yc, float r)
{
    float a = xd*xd + yd*yd;
    float b = 2.0*(xd*(x0 - xc) + yd*(y0 - yc));
    float c = (x0 - xc)*(x0 - xc) + (y0 - yc)*(y0 - yc) - r*r;

    float det = b*b - 4.0*a*c;

    if (det >= 0.0)
        return 1;

    return 0;
}

float fast_tanh(float x)
{
    return x/(1.0 + abs_(x));
}


/*
    result == 0 - tangent
    result > 0.0 intersection
    result < 0.0 no intersection

    http://mathworld.wolfram.com/Circle-LineIntersection.html
*/
float line_circle_intersection(float x1, float y1, float x2, float y2, float xc, float yc, float r)
{
    x1-= xc;
    y1-= yc;

    x2-= xc;
    y2-= yc;

    float dx = x2 - x1;
    float dy = y2 - y1;
    float dr = sqrt(dx*dx + dy*dy);

    float det = x1*y2 - x2*y1;

    float disc = r*r*dr*dr - det;

    return disc;
}


float vect_distance(float *va, float *vb, u32 size)
{
    u32 i;
    float sum = 0.0;
    
    for (i = 0; i < size; i++)
        sum+= (va[i] - vb[i])*(va[i] - vb[i]);

    return sqrt(sum);
}


void vector_normalise(std::vector<float> *vector)
{
    u32 i;

    float min_v = 1000000000000.0;
    float max_v = -min_v;

    for (i = 0; i < vector->size(); i++)
    {
        if ((*vector)[i] < min_v)
            min_v = (*vector)[i];

        if ((*vector)[i] > max_v)
            max_v = (*vector)[i];
    }

    float tmp = max(abs_(min_v), abs_(max_v));

    for (i = 0; i < vector->size(); i++)
        (*vector)[i]/= tmp;
}

//                          -1.0, 1.0, 0.0, 256, 
float map_to_interval(float min_a, float max_a, float min_b, float max_b, float value)
{
    float k = (max_b - min_b)/(max_a - min_a);
    float q = min_b - k*min_a;

    return (k*value + q);
}

void normalise_mat(std::vector<std::vector<float>> *mat)
{
    /*
    float max_v = -1.0;
    float min_v = -1.0;

    u32 i, j;

    for (j = 0; j < mat->size(); j++)
        for (i = 0; i < (*mat)[j].size(); i++)
        {
            max_v = max((*mat)[j][i], max_v);
            min_v = min((*mat)[j][i], min_v);
        }


    if (max_v != min_v)
        for (j = 0; j < mat->size(); j++)
            for (i = 0; i < (*mat)[j].size(); i++)
            { 
                // this->q[j][i]/= max_v;
                (*mat)[j][i] = map_to_interval(min_v, max_v, -1.0, 1.0, (*mat)[j][i]);
            }
    */


    float max_v = 0.0;

    u32 i, j;

    for (j = 0; j < mat->size(); j++)
        for (i = 0; i < (*mat)[j].size(); i++)
        {
            max_v = max(abs_((*mat)[j][i]), max_v);
        }

    
    if (max_v != 0.0)
        for (j = 0; j < mat->size(); j++)
            for (i = 0; i < (*mat)[j].size(); i++)
            { 
                (*mat)[j][i]/= max_v;
            }
}


void vect_print(std::vector<float> v)
{
    u32 i;
    printf("(");
    for (i = 0; i < v.size(); i++)
        printf("%6.3f ", v[i]);
    printf(") ");
}
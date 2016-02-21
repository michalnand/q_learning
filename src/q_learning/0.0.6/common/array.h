#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "common.h"

class CArray
{
  private:
    u32 size;
    float *array;

    std::vector<u32> init_vector;
    std::vector<u32> index_;
    std::vector<float> residuum;

  public:
    CArray(std::vector<u32> init_vector);
    ~CArray();


    float get(std::vector<u32> index);
    float get_f(std::vector<float> index);
    float get_idx(u32 idx);

    void set(std::vector<u32> index, float value);
    void set_f(std::vector<float> index, float value);
    void set_idx(u32 idx, float value);

    std::vector<u32> get_dim();
    u32 get_size();

    std::vector<float> get_residuum();

  private:
    u32 get_idx_(std::vector<u32> index);

};

#endif

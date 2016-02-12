#include "array.h"

CArray::CArray(std::vector<u32> init_vector)
{
  this->init_vector = init_vector;
  this->size = 1;

  u32 i;

  for (i = 0; i < init_vector.size(); i++)
    size*= init_vector[i];

  array = (float*)malloc(size*sizeof(float));
  for (i = 0; i < size; i++)
    array[i] = 0.0;

  for (i = 0; i < init_vector.size(); i++)
    index_.push_back(0.0);
}

CArray::~CArray()
{
  if (array != NULL)
  {
    free(array);
    array = NULL;
  }
}

float CArray::get(std::vector<u32> index)
{
  u32 idx = get_idx_(index);
  return array[idx];
}

float CArray::get_f(std::vector<float> index)
{
  u32 i;
  for (i = 0; i < index_.size(); i++)
  {
    if (index[i] > 0.99999)
      index[i] = 0.99999;

    if (index[i] < 0.0)
      index[i] = 0.0;

    u32 tmp = floor(index[i]*init_vector[i]);
    index_[i] = tmp;
  }

  u32 idx = get_idx_(index_);
  return array[idx];
}

float CArray::get_idx(u32 idx)
{
  return array[idx];
}

void CArray::set(std::vector<u32> index, float value)
{
  u32 idx = get_idx_(index);
  array[idx] = value;
}

void CArray::set_f(std::vector<float> index, float value)
{
  u32 i;
  for (i = 0; i < index_.size(); i++)
  {
    if (index[i] > 0.99999)
      index[i] = 0.99999;

    if (index[i] < 0.0)
      index[i] = 0.0;

    u32 tmp = floor(index[i]*init_vector[i]);
    index_[i] = tmp;
  }

  u32 idx = get_idx_(index_);
  array[idx] = value;
}

void CArray::set_idx(u32 idx, float value)
{
  array[idx] = value;
}

u32 CArray::get_idx_(std::vector<u32> index)
{
  u32 idx = 0;

  u32 i;

  idx = index[0];
  for (i = 1; i < index.size(); i++)
    idx = idx*init_vector[i] + index[i];

  return idx;
}

std::vector<u32> CArray::get_dim()
{
  return init_vector;
}

u32 CArray::get_size()
{
  return size;
}

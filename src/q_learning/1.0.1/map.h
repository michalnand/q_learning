#ifndef _MAP_H_
#define _MAP_H_

#include "common.h"

#define MAP_MAGIC                     (u32)0xABCD0001

#define  MAP_FIELD_NULL                 0
#define  MAP_FIELD_NEGATIVE_REWARD      -1
#define  MAP_FIELD_POSITIVE_REWARD      1
#define  MAP_FIELD_COMMON               2
#define  MAP_FIELD_TARGET               3


struct sMapInit
{
  u32 magic;
  u32 id;
  u32 fields_count;
  float dt;
  float x_size, y_size, z_size;
};

struct sMapField
{
  u32 id;
  i32 type;
  float reward, dist;
  float pos_x, pos_y, pos_z;
  u8 color_r, color_g, color_b, color_a;
};


class CMap
{
  private:
    struct sMapInit map_init;

    std::vector<struct sMapField> fields;

  public:
    CMap(float dt, u32 fields_count, bool make_frame_ = false);
    ~CMap();

    struct sMapField get(float pos_x, float pos_y, float pos_z);

    void save_plot(char *file_name);

    i32 save(char *file_name);
    i32 load(char *file_name);

  private:
    u32 make_frame(u32 id_start);


};

#endif

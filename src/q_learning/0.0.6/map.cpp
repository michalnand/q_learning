#include "map.h"

CMap::CMap(float dt, u32 fields_count)
{
  u32 i;

  map_init.magic = MAP_MAGIC;
  map_init.id = rand();
  map_init.fields_count = fields_count;
  map_init.dt = dt;
  map_init.x_size = 1.0;
  map_init.y_size = 1.0;
  map_init.z_size = 0.0;

  struct sMapField field;
  field.id = 0;
  field.id = make_frame(field.id);

  field.type = MAP_FIELD_COMMON;
  field.reward = 0.0;

  field.pos_x = 0.0;
  field.pos_y = 0.0;
  field.pos_z = 0.0;

  field.color_r = 0;
  field.color_g = 0;
  field.color_b = 0;
  field.color_a = 0;

  for (i = 0; i < fields_count; i++)
  {
    field.pos_x = rnd_();
    field.pos_y = rnd_();
    field.pos_z = 0.0;

    field.reward = -abs_(rnd_());

    u8 cl;
    if ((rand()%100) < 50)
      cl = 10;
    else
      cl = 0;

    field.color_r = cl;
    field.color_g = cl;
    field.color_b = cl;
    field.color_a = 0;

    if (field.reward < 0)
      field.type = MAP_FIELD_NEGATIVE_REWARD;
    else
      field.type = MAP_FIELD_POSITIVE_REWARD;

    fields.push_back(field);
    field.id++;
  }

  field.type = MAP_FIELD_TARGET;
  field.pos_x = rnd_()*0.9;
  field.pos_y = rnd_()*0.9;
  field.pos_z = 0.0;
  field.reward = 1.0;

  fields.push_back(field);


  map_init.fields_count = fields.size();

}

CMap::~CMap()
{

}


struct sMapField CMap::get(float pos_x, float pos_y, float pos_z)
{
  u32 i;
  u32 dist_min_idx = 0;
  float dist_min = 10000000000.0;


  struct sMapField field;
  field.id = 0;
  field.type = MAP_FIELD_NULL;
  field.reward = 0.0;

  field.pos_x = 0.0;
  field.pos_y = 0.0;
  field.pos_z = 0.0;
  field.dist = dist_min;

  field.color_r = 0;
  field.color_g = 0;
  field.color_b = 0;
  field.color_a = 0;

  for (i = 0; i < fields.size(); i++)
  {
    float dist = 0.0;
    dist+= pow(fields[i].pos_x - pos_x, 2.0);
    dist+= pow(fields[i].pos_y - pos_y, 2.0);
    dist+= pow(fields[i].pos_z - pos_z, 2.0);

    dist = sqrt(dist);

    if (dist < dist_min)
    {
      dist_min = dist;
      dist_min_idx = i;
      field.dist = dist;
    }
  }

  float tmp = 1.0;

  if (fields[dist_min_idx].type == MAP_FIELD_TARGET)
    tmp = 2.0;

  if (dist_min < map_init.dt*tmp)
    field = fields[dist_min_idx];

  return field;
}

void CMap::save_plot(char *file_name)
{
  float x, y, z = 0.0;

  CLog log(file_name, 9);

  for (y = -map_init.y_size; y < map_init.y_size; y+= map_init.dt)
  {
    for (x = -map_init.x_size; x < map_init.x_size; x+= map_init.dt)
    {
      struct sMapField field;
      field = get(x, y, z);

      log.add(0, x);
      log.add(1, y);
      log.add(2, field.id);
      log.add(3, field.type);
      log.add(4, field.reward);

      log.add(5, field.color_r);
      log.add(6, field.color_g);
      log.add(7, field.color_b);
      log.add(8, field.color_a);
    }
  }

  log.save();
}


i32 CMap::save(char *file_name)
{
  u32 i;
  FILE *f;

  f = fopen(file_name, "w");
  if (f == NULL)
    return -1;

  fwrite((void*)(&map_init), sizeof(struct sMapInit), 1, f);


  for (i = 0; i < map_init.fields_count; i++)
  {
    struct sMapField field;
    field = fields[i];
    fwrite((void*)(&field), sizeof(struct sMapField), 1, f);
  }

  fclose(f);

  return 0;
}

i32 CMap::load(char *file_name)
{
  u32 i;
  FILE *f;

  fields.clear();
  map_init.fields_count = 0;
  map_init.magic = 0;

  f = fopen(file_name, "r");

  if (f == NULL)
    return -1;

  fread((void*)(&map_init), sizeof(struct sMapInit), 1, f);

  if (map_init.magic != MAP_MAGIC)
    return -2;


  for (i = 0; i < map_init.fields_count; i++)
  {
    struct sMapField field;
    fread((void*)(&field), sizeof(struct sMapField), 1, f);
    fields.push_back(field);
  }

  fclose(f);

  return 0;
}








u32 CMap::make_frame(u32 id_start)
{
  float x, y;

  struct sMapField field;
  field.id = id_start;
  field.type = MAP_FIELD_COMMON;
  field.reward = 0.0;
  field.dist = 10000000000.0;

  for (x = -map_init.x_size; x < map_init.x_size; x+= map_init.dt)
  {
    field.pos_x = x;
    field.pos_y = -map_init.y_size;
    field.pos_z = 0.0;
    field.reward = -1.0;

    field.color_r = 0;
    field.color_g = 0;
    field.color_b = 0;
    field.color_a = 0;

    field.type = MAP_FIELD_NEGATIVE_REWARD;
    fields.push_back(field);
    field.id++;
  }

  for (x = -map_init.x_size; x < map_init.x_size; x+= map_init.dt)
  {
    field.pos_x = x;
    field.pos_y = map_init.y_size;
    field.pos_z = 0.0;
    field.reward = -1.0;

    field.color_r = 0;
    field.color_g = 0;
    field.color_b = 0;
    field.color_a = 0;

    field.type = MAP_FIELD_NEGATIVE_REWARD;
    fields.push_back(field);
    field.id++;
  }


  for (y = -map_init.y_size; y < map_init.y_size; y+= map_init.dt)
  {
    field.pos_x = -map_init.x_size;
    field.pos_y = y;
    field.pos_z = 0.0;
    field.reward = -1.0;

    field.color_r = 0;
    field.color_g = 0;
    field.color_b = 0;
    field.color_a = 0;

    field.type = MAP_FIELD_NEGATIVE_REWARD;
    fields.push_back(field);
    field.id++;
  }

  for (y = -map_init.y_size; y < map_init.y_size; y+= map_init.dt)
  {
    field.pos_x = map_init.x_size;
    field.pos_y = y;
    field.pos_z = 0.0;
    field.reward = -1.0;

    field.color_r = 0;
    field.color_g = 0;
    field.color_b = 0;
    field.color_a = 0;

    field.type = MAP_FIELD_NEGATIVE_REWARD;
    fields.push_back(field);
    field.id++;
  }

  return field.id;
}

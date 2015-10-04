#include "map.h"


/*

	struct sMapField
	{
		u32 type;
		u32 id;
		u32 texture_id;

		i32 parameter_int;
		float parameter_f;

		float reward;
		float position[4];
		float color[4];

		void *next_info_ptr;
	};


	struct sMap
	{
		u32 type;
		u32 id;

		u32 width, height;

		float base_width, base_height;

		struct sMapField **fields;

		void *next_info_ptr;
	};

*/


CMap::CMap(	u32 type, u32 id, u32 width, u32 height,
			 	float base_width, float base_height,
			 	void *next_info_ptr)
{

	map.magic = MAP_MAGIC;
	map.type = type;
	map.id = id;

	map.width = width;
	map.height = height;

	map.base_width = base_height;
	map.base_width = base_width;

	map.next_info_ptr = next_info_ptr;


	struct sMapField field;

	field.type = MAP_FIELD_TYPE_EMPTY;
	field.id = 0;
	field.texture_id = 0;

	field.parameter_int = 0;
	field.parameter_f = 0.0;

	field.reward = 0.0;

	field.position[0] = 0.0;
	field.position[1] = 0.0;
	field.position[2] = 0.0;
	field.position[3] = 0.0;

	field.color[0] = 0.0;
	field.color[1] = 0.0;
	field.color[2] = 0.0;
	field.color[3] = 0.0;

	field.next_info_ptr = NULL;


	u32 j, i;

	map.fields = (struct sMapField**)malloc(map.width*sizeof(struct sMapField*));
	for (j = 0; j < map.height; j++)
	{
		map.fields[j] = (struct sMapField*)malloc(map.width*sizeof(struct sMapField));

		for (i = 0; i < map.width; i++)
		{
			field.position[0] = i;
			field.position[1] = j;

			map.fields[j][i] = field;
			field.id++;
		}
	}
	printf("map size %i %i\n", map.height, map.width);
}

CMap::~CMap()
{

}


void CMap::init(	u32 type, u32 id, u32 width, u32 height,
			 		float base_width, float base_height,
			 		void *next_info_ptr)
{
	u32 j, i;

	if (map.fields != NULL)
	{
		for (j = 0; j < map.height; j++)
			if (map.fields[j] != NULL)
			{
				free (map.fields[j]);
				map.fields[j] = NULL;
			}

		free (map.fields);
		map.fields = NULL;
	}

	map.type = type;
	map.id = id;

	map.width = width;
	map.height = height;

	map.base_width = base_height;
	map.base_width = base_width;

	map.next_info_ptr = next_info_ptr;

	struct sMapField field;

	field.type = MAP_FIELD_TYPE_EMPTY;
	field.id = 0;
	field.texture_id = 0;

	field.parameter_int = 0;
	field.parameter_f = 0.0;

	field.reward = 0.0;

	field.position[0] = 0.0;
	field.position[1] = 0.0;
	field.position[2] = 0.0;
	field.position[3] = 0.0;

	field.color[0] = 0.0;
	field.color[1] = 0.0;
	field.color[2] = 0.0;
	field.color[3] = 0.0;

	field.next_info_ptr = NULL;

	map.fields = (struct sMapField**)malloc(map.width*sizeof(struct sMapField*));

	for (j = 0; j < map.height; j++)
	{
		map.fields[j] = (struct sMapField*)malloc(map.width*sizeof(struct sMapField));

		for (i = 0; i < map.width; i++)
		{
			field.position[0] = i;
			field.position[1] = j;

			map.fields[j][i] = field;
			field.id++;
		}
	}
}


i32 CMap::save(char *file_name)
{
	FILE *f;

	f = fopen(file_name, "w");

	if (f == NULL)
		return -1;

  	fwrite (&map, sizeof(struct sMap), 1, f);

  	u32 j, i;

  	for (j = 0; j < map.height; j++)
  		for (i = 0; i < map.width; i++)
  			fwrite (&map.fields[j][i], sizeof(struct sMapField), 1, f);
	fclose(f);

	printf("saving done into %s\n", file_name);
	return 0;
}

i32 CMap::load(char *file_name)
{
	FILE *f;
	u32 j, i;
	u32 read_res;

	f = fopen(file_name, "r");
	if (f == NULL)
		return -1;

	if (map.fields != NULL)
	{
		for (j = 0; j < map.height; j++)
			if (map.fields[j] != NULL)
			{
				free (map.fields[j]);
				map.fields[j] = NULL;
			}

		free (map.fields);
		map.fields = NULL;
	}

	read_res = fread(&map, sizeof(map), 1, f);

	printf("map loaded %x %i %i\n", map.magic, map.width, map.height);

	map.fields = (struct sMapField**)malloc(map.width*sizeof(struct sMapField*));
	for (j = 0; j < map.height; j++)
	{
		map.fields[j] = (struct sMapField*)malloc(map.width*sizeof(struct sMapField));

		for (i = 0; i < map.width; i++)
		{
			read_res = fread(&map.fields[j][i], sizeof(struct sMapField), 1, f);

			if (map.fields[j][i].reward > 1.0)
				map.fields[j][i].reward = 1.0;

			if (map.fields[j][i].reward < -1.0)
				map.fields[j][i].reward = -1.0;
		}
	}

	fclose(f);

	if (read_res == 0)
		return -2;

	return 0;
}

void CMap::print()
{
	u32 i, j;

	printf("fields : \n");
	for (j = 0; j < map.height; j++)
	{
		for (i = 0; i < map.width; i++)
			printf("%3u ", map.fields[j][i].type);
		printf("\n");
	}
	printf("\n\n");


	printf("rewards : \n");
	for (j = 0; j < map.height; j++)
	{
		for (i = 0; i < map.width; i++)
			printf("%5.1f ", map.fields[j][i].reward);
		printf("\n");
	}
	printf("\n\n");

}

struct sMapField CMap::get_at(u32 x, u32 y)
{
	return map.fields[y][x];
}

u32 CMap::set_at(u32 x, u32 y, struct sMapField field)
{
	if (x > map.width)
		return 0;

	if (y > map.height)
		return 0;

	map.fields[y][x] = field;

	return 1;
}

struct sMapField CMap::get_at_normalised(float x, float y)
{
	if (x > 1.0)
		x = 1.0;
	if (x < -1.0)
		x = -1.0;

	if (y > 1.0)
		y = 1.0;
	if (y < -1.0)
		y = -1.0;

	x = (x + 1.0)/2.0;
	y = (y + 1.0)/2.0;

	if (x < 0.0)
		x = 0.0;
	if (x > 0.999)
		x = 0.999;

	if (y < 0.0)
		y = 0.0;
	if (y > 0.999)
		y = 0.999;




	u32 x_ = (u32)(map.width*x);
	u32 y_ = (u32)(map.height*y);
	struct sMapField res = map.fields[y_][x_];

	return res;
}

u32 CMap::set_at_normalised(float x, float y, struct sMapField field)
{
	if (x > 1.0)
		x = 1.0;
	if (x < -1.0)
		x = -1.0;

	if (y > 1.0)
		y = 1.0;
	if (y < -1.0)
		y = -1.0;

	u32 x_ = map.width*(x + 1.0)/2.001;
	u32 y_ = map.height*(y + 1.0)/2.001;

	if (x_ > map.width)
		return 0;

	if (y_ > map.height)
		return 0;

	map.fields[y_][x_] = field;

	return 1;
}


u32 CMap::get_height()
{
	return map.height;
}

u32 CMap::get_width()
{
	return map.width;
}

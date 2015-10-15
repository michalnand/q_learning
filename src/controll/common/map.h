#ifndef _MAP_H_
#define _MAP_H_

#include "common.h"

#define MAP_FIELD_TYPE_EMPTY		(u32)0
#define MAP_FIELD_TYPE_WALL			(u32)1

#define MAP_FIELD_TYPE_PATH			(u32)2
#define MAP_FIELD_TYPE_TARGET		(u32)3
#define MAP_FIELD_TYPE_SOURCE		(u32)4
#define MAP_FIELD_TYPE_DESTINATION	(u32)5

#define MAP_FIELD_TYPE_RED_ROBOT	(u32)6
#define MAP_FIELD_TYPE_RED_TARGET	(u32)7
#define MAP_FIELD_TYPE_RED_PATH		(u32)8

#define MAP_FIELD_TYPE_GREEN_ROBOT	(u32)9
#define MAP_FIELD_TYPE_GREEN_TARGET	(u32)10
#define MAP_FIELD_TYPE_GREEN_PATH	(u32)11

#define MAP_FIELD_TYPE_BLUE_ROBOT	(u32)12
#define MAP_FIELD_TYPE_BLUE_TARGET	(u32)13
#define MAP_FIELD_TYPE_BLUE_PATH	(u32)14


#define MAP_MAGIC					(u32)0xFFAB2357

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
	u32 magic;

	u32 type;
	u32 id;

	u32 width, height;

	float base_width, base_height;

	struct sMapField **fields;

	void *next_info_ptr;
};


class CMap
{
	private:

		struct sMap map;

	public:
		CMap(	u32 type, u32 id, u32 width = 34, u32 height = 19,
			 	float base_width = 55.0, float base_height = 55.0,
			 	void *next_info_ptr = NULL);

		~CMap();

		void init(	u32 type, u32 id, u32 width = 34, u32 height = 19,
			 		float base_width = 55.0, float base_height = 55.0,
			 		void *next_info_ptr = NULL);

		i32 save(char *file_name);
		i32 load(char *file_name);

		struct sMapField get_at(u32 x, u32 y);
		u32 set_at(u32 x, u32 y, struct sMapField field);

		struct sMapField get_at_normalised(float x, float y);
		u32 set_at_normalised(float x, float y, struct sMapField field);

		u32 get_height();
		u32 get_width();

		void print();
};

#endif

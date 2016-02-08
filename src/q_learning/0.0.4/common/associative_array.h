#ifndef _ASSOCIATIVE_ARRAY_H_
#define _ASSOCIATIVE_ARRAY_H_

#include "common.h"


class CAssociativeArray
{
	private:

		std::vector<std::vector<float>> array;

	public:
		CAssociativeArray(u32 dimensions, float density);
		~CAssociativeArray();

		u32 get_size();
		u32 get(std::vector<float> input);
		void print();
};

#endif
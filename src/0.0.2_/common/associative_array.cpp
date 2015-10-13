#include "associative_array.h"

CAssociativeArray::CAssociativeArray(u32 dimensions, float density)
{
	u32 i;

	std::vector<float> tmp;

	for (i = 0; i < dimensions; i++)
		tmp.push_back(-1.0);

	do
	{
		array.push_back(tmp);

		tmp[0]+= density;

		for (i = 0; i < (dimensions-1); i++)
			if (tmp[i] >= 1.0)
			{
				tmp[i] = -1.0;
				tmp[i+1]+= density;
			}
	}
		while (tmp[dimensions-1] < 1.0);
}

CAssociativeArray::~CAssociativeArray()
{

}

u32 CAssociativeArray::get_size()
{
	return array.size();
}

u32 CAssociativeArray::get(std::vector<float> input)
{
	u32 j, i, min_idx = 0;
	float min_dist = array[0].size()*10.0;

	for (j = 0; j < array.size(); j++)
	{
		float dist = 0.0;
		for (i = 0; i < array[j].size(); i++)
			dist+= abs_(array[j][i] - input[i]);

		if (dist < min_dist)
		{
			min_idx = j;
			min_dist = dist;
		}
	}

	return min_idx;
}


void CAssociativeArray::print()
{
	u32 j, i;

	for (j = 0; j < array.size(); j++)
	{
		for (i = 0; i < array[j].size(); i++)
			printf("%6.3f ", array[j][i]);

		printf("\n");
	}

	printf("\n\n");
}

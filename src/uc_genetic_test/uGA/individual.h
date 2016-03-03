#ifndef _INDIVIDUAL_H_
#define _INDIVIDUAL_H_

#include "../common.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "../int_def.h"

template <class fitness_class> class CIndividual
{
		fitness_class *fitness_function;
		std::vector<float> solution, solution_testing;

		float fitness_value;
		u32 solution_size;

	public:

		CIndividual()
		{
			this->fitness_function = new fitness_class();

			solution_size = this->fitness_function->get_size();

			u32 i;
			for (i = 0; i < solution_size; i++)
			{
				solution.push_back(rnd());
				solution_testing.push_back(solution[i]);
			} 
		}

		~CIndividual()
		{
			delete this->fitness_function;
		}

		void process()
		{
			return fitness_function->process(solution_testing);
		}

		void reset()
		{
			u32 i;
			for (i = 0; i < solution.size(); i++)
				solution[i] = rnd();
		}

		void set(std::vector<float> solution)
		{
			u32 i;
			for (i = 0; i < solution.size(); i++)
				solution_testing[i] = solution[i];
		}

		void set_at(u32 idx, float value)
		{
			solution_testing[idx] = value;
		}

		void mutate()
		{
			u32 idx = rand()%solution_testing.size();
			solution_testing[idx]+= 0.1*rnd();

			if (solution_testing[idx] > 1.0)
				solution_testing[idx] = 1.0;

			if (solution_testing[idx] < -1.0)
				solution_testing[idx] = -1.0;
		}

		std::vector<float>get()
		{
			return solution;
		}

		float get_fitness()
		{
			return fitness_function->get_fitness();
		}

		void cross(std::vector<float> pa, std::vector<float> pb)
		{
			u32 i;
			for (i = 0; i < solution.size(); i++)
			{
				if (rnd() < 0.0)
					solution_testing[i] = pa[i];
				else
					solution_testing[i] = pb[i];
			}
		}

		void accept()
		{
			u32 i;
			for (i = 0; i < solution.size(); i++)
				solution[i] = solution_testing[i];
		}

		void print()
		{
			printf("%6.5f : ", fitness_function->get_fitness());
				
			u32 i;
			for (i = 0; i < solution.size(); i++)
				printf("%6.3f ", solution[i]);

			printf("\n");
		}

	private:
	
		float rnd()
		{
			return ((rand()%20000000) - 10000000)/10000000.0;
		}
};

#endif
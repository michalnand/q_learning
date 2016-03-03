#ifndef _GENENETIC_H_
#define _GENENETIC_H_

#include <thread>
#include <vector>
#include "individual.h"


template <class fitness_class> class CGenetic
{
	private:
		std::vector<CIndividual<fitness_class>*> individuals;

		u32 best_fitness_idx;
		float best_fitness_value;

		bool paralel;

	public:
		
		/*
		CGenetic(u32 population_size = 5);
		~CGenetic();

		void process();
		void print();
		*/

		CGenetic(u32 population_size, std::vector<float> initial_solution, bool paralel = false)
		{
			u32 i;
			for (i = 0; i < population_size; i++)
				individuals.push_back( new CIndividual<fitness_class>() );

			individuals[0]->set(initial_solution);
			individuals[0]->accept();

			best_fitness_idx = 0;
			best_fitness_value = individuals[0]->get_fitness();

			this->paralel = paralel;
		}

		~CGenetic()
		{
			u32 i;
			for (i = 0; i < individuals.size(); i++)
				delete individuals[i]; 
		}

		void individual_process(u32 id)
		{
			individuals[id]->process();
		}

		void process()
		{
			u32 i;

			if (paralel == true)
			{		
				std::vector<std::thread> paralel_threads;

    			for (i = 0; i < individuals.size(); i++)
    			{
    				paralel_threads.push_back(
    					std::thread(&CGenetic<fitness_class>::individual_process, this, i) );
    			}

				for (i = 0; i < individuals.size(); i++)
					paralel_threads[i].join();
			}
			else
			for (i = 0; i < individuals.size(); i++)
				individual_process(i);

			best_fitness_idx = 0;

			float fitness_average = 0.0;

			for (i = 0; i < individuals.size(); i++)
			{
				if (individuals[i]->get_fitness() <= individuals[best_fitness_idx]->get_fitness())
				{
					best_fitness_idx = i;
					best_fitness_value = individuals[i]->get_fitness();
				}

				fitness_average+= individuals[i]->get_fitness();
			}

			fitness_average/= individuals.size();

			float dif = best_fitness_value - fitness_average;
			if (dif < 0.0)
				dif = -dif;

			srand(time(NULL)); 

			// printf("DIF = %f AVERAGE = %f\n", dif, fitness_average);

			if (dif < fitness_average/10.0)
			{
				for (i = 0; i < individuals.size(); i++)
					if (i!= best_fitness_idx)
						individuals[i]->reset();

				printf("reseting population\n");
			}

			for (i = 0; i < individuals.size(); i++)
				if (i!= best_fitness_idx)
				{
					u32 pa_idx = rand()%individuals.size();
					u32 pb_idx = rand()%individuals.size();

					individuals[i]->cross(individuals[pa_idx]->get(), individuals[pb_idx]->get());

					if ((rand()%10) == 0)
						individuals[i]->mutate();
					
				}

			for (i = 0; i < individuals.size(); i++)
				individuals[i]->accept();
		}

		void print()
		{
			u32 i;

			printf("\n");

			for (i = 0; i < individuals.size(); i++)
			{
				if (i == best_fitness_idx)
					printf("* ");
				else
					printf("  ");

				individuals[i]->print();
			}

			printf("\n");
		}

		float get_fitness()
		{
			return individuals[best_fitness_idx]->get_fitness();
		}

		std::vector<float> get()
		{
			return individuals[best_fitness_idx]->get();	
		}
};


#endif
#ifndef _Q_LEARNING_TEST_H_
#define _Q_LEARNING_TEST_H_

#include "environment.h"
#include <thread>

class CQlearningTest
{
	private:
		class CEnvironment *environment, *environment_visualisation;

	public:
		CQlearningTest();
		~CQlearningTest();

		void run(u32 iterations = 100);

		void visualisation_main();
};

#endif

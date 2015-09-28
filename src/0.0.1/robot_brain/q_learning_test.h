#ifndef _Q_LEARNING_TEST_H_
#define _Q_LEARNING_TEST_H_

#include "environment.h"

class CQlearningTest
{
	private:
		class CEnvironment *environment;

	public:
		CQlearningTest();
		~CQlearningTest();

		void run(u32 iterations = 100);
};

#endif
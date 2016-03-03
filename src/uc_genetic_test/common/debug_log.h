#ifndef _DEBUG_LOG_H_
#define _DEBUG_LOG_H_

#include "common.h"

#ifndef FILENAME_MAX
#define FILENAME_MAX	256
#endif


struct sDebugLog
{
	char file_name[FILENAME_MAX+1];
	FILE *fd;

	pthread_mutex_t mutex;
};

void debug_log_init(char *file_name);
void debug_log_add(char *s);


#endif
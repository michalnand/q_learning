#include "debug_log.h"


struct sDebugLog g_debug_log;

void debug_log_init(char *file_name)
{ 
	u32 i = 0;

	while ( (i < FILENAME_MAX) && (file_name[i] != '\0') )
	{
		g_debug_log.file_name[i] = file_name[i];
		i++;
	}

	g_debug_log.file_name[FILENAME_MAX] = '\0';

	pthread_mutex_init(&g_debug_log.mutex, NULL);
}

void debug_log_add(char *s)
{
	pthread_mutex_lock(&g_debug_log.mutex);

	time_t clk = time(NULL);

	printf("\nDEBUG LOG : %s : %s\n\n", ctime(&clk), s);

	/*
	g_debug_log.fd = fopen(g_debug_log.file_name, "a+");

	if (g_debug_log.fd != NULL)
	{
		fprintf(g_debug_log.fd, "%s : %s\n\n", ctime(&clk), s);
		fclose(g_debug_log.fd);
	}
	*/

	pthread_mutex_unlock(&g_debug_log.mutex);
}
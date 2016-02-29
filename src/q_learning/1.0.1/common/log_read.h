#ifndef _LOG_READ_H_
#define _LOG_READ_H_


#include "common.h"

class CLogRead
{
	private:
		FILE *file;
		std::vector<std::vector<float>> log_data;

	public:
		CLogRead(char *file_name);
		~CLogRead();

    u32 get_lines_count();
    u32 get_items_count();
    std::vector<float> get(u32 line);
    float get_item(u32 line, u32 item);
};

#endif

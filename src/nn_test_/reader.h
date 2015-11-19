#ifndef _READER_H_
#define _READER_H_

#include "common.h"
#include <fstream>
#include <iostream>

class CReader
{
    private:
        float num_read_columns;
        float num_file_columns;

        std::vector<uint32_t> vec_columns;

        std::vector<std::vector<float>> columns_content;
        std::ifstream fs;

    public:
        CReader(std::string fname, std::vector<uint32_t> columns);
        void read();
        std::vector<std::vector<float>> get();
        ~CReader();
};

#endif

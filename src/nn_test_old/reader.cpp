#include <fstream>
#include <sstream>
#include "reader.h"

CReader::CReader(std::string file_name, std::vector<uint32_t> columns)
{
    this->vec_columns = columns;
    this->num_read_columns = columns.size();
    fs.open(file_name);
    if(fs.is_open()==false)
    {
        std::cout << "file was not opened" << std::endl;
    }
    //number of columns from first row
    std::string first_line;
    std::getline(fs, first_line);
    std::istringstream iss(first_line);
    //get seek at the beginning
    uint32_t frc = 0;
    float tmp;
    while(iss >> tmp)
    {
        frc++;
    }
    fs.seekg (0, fs.beg);
    this->num_file_columns = frc;
}

CReader::~CReader()
{
    fs.close();
}

void CReader::read()
{
    std::vector<std::vector<float>> column(num_read_columns);
    while(!fs.eof())
    {
        uint32_t k = 0;
        for(uint32_t i = 0; i<num_file_columns; ++i)
        {
            float line_element;
            fs >> line_element;
            if(fs.eof())
            {
                break;
            }
            //std::cout << " Line element " << line_element << std::endl;
            //nacitavam aj tie co nechcem
            for(uint32_t j = 0;j< vec_columns.size(); ++j)
            {
                if(i == vec_columns[j])
                {
                    //std::cout << "vec_columns[j] " << vec_columns[j] << std::endl;
                    //std::cout << "i " << i << std::endl;
                    //std::cout << "j " << j << std::endl;
                    column[k].push_back(line_element);
                    k++;
                }
            }

        }
    }
    columns_content = column;
}

std::vector<std::vector<float>> CReader::get()
{
    return this->columns_content;
}

#include "log_read.h"

u32 is_float_symbol(char c)
{
  if ((c >= '0') && (c <= '9'))
    return 1;

  if (c == '-')
    return 2;

  if (c == '.')
    return 3;

  return 0;
}

CLogRead::CLogRead(char *file_name)
{
  file = fopen(file_name,"r");

  if (file == NULL)
  {
    printf("log loading error : %s\n", file_name);
    return;
  }

  char buffer[10000];

  while (!feof(file))
  {
    if (fgets (buffer, 10000, file) == 0)
      break;

    std::vector<float> tmp;
    u32 ptr = 0;

    while ((buffer[ptr] != '\0') && (buffer[ptr] != '\n'))
    {
      tmp.push_back(atof(buffer + ptr));
      while ( is_float_symbol(buffer[ptr]) &&
              (buffer[ptr]!='\0') &&
              (buffer[ptr] != '\n'))
          ptr++;

      while (buffer[ptr]== ' ')
        ptr++;
    }

    log_data.push_back(tmp);
  }

  fclose(file);
}

CLogRead::~CLogRead()
{

}

u32 CLogRead::get_lines_count()
{
  return log_data.size();
}

u32 CLogRead::get_items_count()
{
  return log_data[0].size();
}

std::vector<float> CLogRead::get(u32 line)
{
  return log_data[line];
}

float CLogRead::get_item(u32 line, u32 item)
{
  return log_data[line][item];
}

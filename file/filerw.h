#ifndef FILERW_H
#define FILERW_H

#include <stddef.h>

int save_file(char const* file_name, char const* buff, size_t size);
int open_file(char const* file_name, char* buff, size_t size);

#endif

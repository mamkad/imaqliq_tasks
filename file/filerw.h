#ifndef FILERW_H
#define FILERW_H

int save_file(char const* file_name, char const* buff, long int size)
char* open_file(char const* file_name, long int* size);

#endif

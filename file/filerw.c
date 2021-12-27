#include "filerw.h"

#include <stdlib.h>
#include <stdio.h>

int save_file(char const* file_name, char const* buff, size_t size)
{
	FILE* file = fopen(file_name, "wb");
	if (file == NULL) {
	 	return -1;
	}
	 
	size_t nbytes = fwrite(buff, sizeof(char), size, file);
	fclose(file);
	
	return nbytes;
}

int open_file(char const* file_name, char* buff, size_t size)
{
	FILE * file = fopen(file_name , "rb");
	if (file == NULL) {
	 	return -1;
	}
	
    int ret_code = fseek(file , 0 , SEEK_END);   
    if (ret_code != 0) {
    	fclose (file);
    	return -1;
    }
     
    size_t nsize = ftell(file);  
    if (nsize == -1L) {
    	fclose (file);
    	return -1;
    }
     
    ret_code = fseek(file , 0 , SEEK_SET); 
    if (ret_code != 0) {
    	fclose (file);
    	return -1;
    }                 
 	
  	size_t nbytes = fread(buff, sizeof(char), size, file);       
  	fclose (file);
 
  	return nbytes;
}

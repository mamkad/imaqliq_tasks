#include "filerw.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILELENGTHSIZE 17

int save_file(char const* file_name, char const* buff, long int size)
{
	FILE* file = fopen(file_name, "wb");
	if (file == NULL) {
	 	return -1;
	}
	 
	size_t nbytes = fwrite(buff, sizeof(char), size, file);
	fclose(file);
	
	return nbytes;
}

char* open_file(char const* file_name, long int* size)
{
	FILE * file = fopen(file_name , "rb");
	if (file == NULL) {
	 	return NULL;
	}
	
    int ret_code = fseek(file , 0 , SEEK_END);   
    if (ret_code != 0) {
    	fclose (file);
    	return NULL;
    }
     
    *size = ftell(file);  
    if (*size == -1L) {
    	fclose (file);
    	return NULL;
    }
     
    ret_code = fseek(file , 0 , SEEK_SET); 
    if (ret_code != 0) {
    	fclose (file);
    	return NULL;
    }
    
    char size_number[FILELENGTHSIZE];
    size_number[0] = '\0';
    
    int cur_number_size = sprintf(size_number, "%ld", *size);
    (*size) += FILELENGTHSIZE;
    
    char * buff = (char*)malloc(sizeof(char) * (*size));
    if (buff == NULL) {
    	fclose (file);
    	return NULL;
    }   
    strncpy(buff, size_number, cur_number_size);
    memset(buff + cur_number_size, ' ', FILELENGTHSIZE - cur_number_size - 1);
    buff[FILELENGTHSIZE - 1] = '\n';

  	size_t nbytes = fread(buff + FILELENGTHSIZE, sizeof(char), (*size), file);       
  	fclose (file);
  	return buff;
}

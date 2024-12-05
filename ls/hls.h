#ifndef HLS_H
#define HLS_H
#include <stddef.h>  

int read_directory(DIR *dir, char ***filenames, size_t *count, size_t *capacity)';'
int resize_filenames(char ***filenames, size_t *count, size_t *capacity);

#endif 

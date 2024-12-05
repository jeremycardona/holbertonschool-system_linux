#ifndef HLS_H
#define HLS_H

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

int read_directory(DIR *, char ***filenames, size_t *count, size_t *capacity);
int resize_filenames(char ***filenames, size_t *count, size_t *capacity);


#endif

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>  

size_t my_strlen(const char *str);
char *my_strcpy(char *dest, const char *src);
int my_strcmp(const char *str1, const char *str2);
void sort_filenames(char **filenames, size_t count);

#endif 
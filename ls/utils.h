#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

size_t my_strlen(const char *str);
char *my_strcpy(char *dest, const char *src);
char my_tolower(char c);
int my_strcmp(const char *str1, const char *str2);
int my_strcasecmp(const char *str1, const char *str2);
char *my_strdup(const char *str);
void sort_filenames(char **filenames, size_t count, int options);

#endif /* UTILS_H */
#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "utils.h"
#include "hls.h"

void print_error(const char *prog_name, const char *dir, int is_permission_error);
void print_file_info(const char *dir_name, const char *file_name);
void print_directory_contents(const char *dir_name, char **filenames, size_t count, int options);
int process_directory(char *argv[], const char *dir_name, int options, int is_multiple_dirs);
int process_arguments(int argc, char *argv[], int options);

#endif /* PARAMETERS_H */
#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "utils.h"
#include "hls.h"

void print_error(const char *prog_name, const char *dir);
void print_directory_contents(char **filenames, size_t count);
int process_directory(const char *dir_name);
int process_arguments(int argc, char *argv[]);

#endif /* PARAMETERS_H */

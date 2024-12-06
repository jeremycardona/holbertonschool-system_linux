#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "utils.h"
#include "hls.h"
#include "parameters.h"
#include "options.h"

int main(int argc, char *argv[])
{
    int options = 0;
    int file_start_index = parse_options(argc, argv, &options);

    if (file_start_index == argc)
    {
        fprintf(stderr, "%s: missing operand\n", argv[0]);
        perror("missing operand");
        return EXIT_FAILURE;
    }

    int result = process_arguments(argc, argv, options);

    if (result == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    return (0);
}
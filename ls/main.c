#include <stdio.h>
#include <stdlib.h>
#include "options.h"
#include "parameters.h"

/**
 * main - List and sort the contents of the given directories or files.
 * @argc: Argument count.
 * @argv: Argument vector.
 *
 * Description: Program that lists the contents of the given directories
 * or files in alphabetical order.
 * Return: Always 0.
 */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "%s: missing operand\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int options = 0;
    int file_start_index = parse_options(argc, argv, &options);

    if (file_start_index == argc)
    {
        fprintf(stderr, "%s: missing operand\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int result = process_arguments(argc - file_start_index, &argv[file_start_index], options);

    if (result == EXIT_FAILURE)
    {
        exit(EXIT_FAILURE);
    }

    return (0);
}
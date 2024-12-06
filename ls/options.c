#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "options.h"

/**
 * parse_options - Parses command-line options.
 * @argc: Argument count.
 * @argv: Argument vector.
 * @options: Pointer to the options bitmask.
 *
 * Return: The index of the first non-option argument.
 */
int parse_options(int argc, char *argv[], int *options)
{
    int i, j;
    int first_non_option_index = -1;

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (j = 1; argv[i][j] != '\0'; j++)
            {
                switch (argv[i][j])
                {
                case '1':
                    *options |= OPT_ONE_PER_LINE;
                    break;
                case 'a':
                    *options |= OPT_ALL;
                    break;
                case 'A':
                    *options |= OPT_ALMOST_ALL;
                    break;
                case 'l':
                    *options |= OPT_LONG_FORMAT;
                    break;
                case 'r':
                    *options |= OPT_REVERSE;
                    break;
                case 'S':
                    *options |= OPT_SORT_SIZE;
                    break;
                case 't':
                    *options |= OPT_SORT_TIME;
                    break;
                case 'R':
                    *options |= OPT_RECURSIVE;
                    break;
                default:
                    fprintf(stderr, "hls: invalid option -- '%c'\n", argv[i][j]);
                    perror("invalid option");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            if (first_non_option_index == -1)
            {
                first_non_option_index = i;
            }
        }
    }

    return (first_non_option_index == -1) ? argc : first_non_option_index;
}
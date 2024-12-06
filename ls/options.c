#include "options.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * parse_options - Parses command line options.
 * @argc: The number of arguments.
 * @argv: The array of arguments.
 * @options: Pointer to the options bitmask.
 *
 * Return: The index of the first non-option argument.
 */
int parse_options(int argc, char *argv[], int *options)
{
    int i;
    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (int j = 1; argv[i][j] != '\0'; j++)
            {
                switch (argv[i][j])
                {
                case 'l':
                    *options |= OPT_LONG_FORMAT;
                    break;
                case 'a':
                    *options |= OPT_ALL;
                    break;
                case 'A':
                    *options |= OPT_ALMOST_ALL;
                    break;
                case 'r':
                    *options |= OPT_REVERSE;
                    break;
                case 't':
                    *options |= OPT_SORT_TIME;
                    break;
                case 'S':
                    *options |= OPT_SORT_SIZE;
                    break;
                case '1':
                    *options |= OPT_ONE_PER_LINE;
                    break;
                default:
                    fprintf(stderr, "Unknown option: -%c\n", argv[i][j]);
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            break;
        }
    }
    return i;
}
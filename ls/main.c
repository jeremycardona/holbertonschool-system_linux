#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "utils.h"
#include "hls.h"
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

    int multiple_dirs = (argc > 2);

    for (int i = 1; i < argc; i++)
    {
        DIR *dir = opendir(argv[i]);
        if (!dir)
        {
            print_error(argv[0], argv[i]);
            continue;
        }

        char **filenames = malloc(sizeof(char *) * 10);
        size_t count = 0, capacity = 10;

        if (read_directory(dir, &filenames, &count, &capacity) == -1)
        {
            closedir(dir);
            free(filenames);
            exit(EXIT_FAILURE);
        }

        closedir(dir);

        sort_filenames(filenames, count);

        print_directory_contents(argv[i], filenames, count, multiple_dirs);
    }

    return (0);
}
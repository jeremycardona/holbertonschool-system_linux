#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "hls.h"
#include "utils.h"
#include "options.h"

/**
 * strerror_custom - Custom strerror function to handle errors.
 * @errnum: The error number.
 *
 * Return: A string corresponding to the error.
 */
const char *strerror_custom(int errnum)
{
    switch (errnum)
    {
    case EACCES:
        return ("Permission denied");
    case ENOENT:
        return ("No such file or directory");
    case ENOTDIR:
        return ("Not a directory");
    case EISDIR:
        return ("Is a directory");
    case ENOMEM:
        return ("Out of memory");
    case EEXIST:
        return ("File exists");
    case ENAMETOOLONG:
        return ("File name too long");
    default:
        return ("Unknown error");
    }
}

/**
 * print_error - Prints an error message.
 * @prog_name: The name of the program.
 * @dir: The directory or file causing the error.
 * @is_permission_error: Flag indicating if the error is a permission issue.
 */
void print_error(const char *prog_name, const char *dir, int is_permission_error)
{
    if (errno == ENOENT)
    {
        fprintf(stderr, "%s: cannot access %s: %s\n", prog_name, dir, strerror_custom(errno));
    }
    else if (is_permission_error)
    {
        fprintf(stderr, "%s: cannot open directory %s: %s\n", prog_name, dir, strerror_custom(errno));
    }
    else
    {
        fprintf(stderr, "%s: cannot access %s: %s\n", prog_name, dir, strerror_custom(errno));
    }
}

/**
 * print_directory_contents - Prints the directory contents.
 * @dir_name: The name of the directory.
 * @filenames: Array of filenames.
 * @count: The number of filenames.
 * @options: Options bitmask.
 */
void print_directory_contents(const char *dir_name, char **filenames, size_t count, int options)
{
    if (options & OPT_RECURSIVE)
    {
        printf("%s:\n", dir_name);  /* Print the directory name only if multiple directories are handled */
    }

    for (size_t j = 0; j < count; j++)
    {
        printf("%s", filenames[j]);
        if (options & OPT_ONE_PER_LINE)
        {
            printf("\n");
        }
        else
        {
            printf("  ");
        }
        free(filenames[j]);
    }
    free(filenames);

    if (!(options & OPT_ONE_PER_LINE))
    {
        printf("\n");  /* Print a newline after each directory's contents when not using -1 option */
    }
}

/**
 * process_directory - Processes a directory and lists its contents.
 * @dir_name: The directory name.
 * @options: Options bitmask.
 *
 * Return: 0 if success, -1 if error.
 */
int process_directory(const char *dir_name, int options)
{
    struct stat statbuf;

    if (stat(dir_name, &statbuf) == -1)
    {
        print_error("./hls_01", dir_name, 0);
        return (-1);
    }

    if (S_ISDIR(statbuf.st_mode))
    {
        DIR *dir = opendir(dir_name);

        if (!dir)
        {
            print_error("./hls_01", dir_name, 1);
            return (-1);
        }

        char **filenames = malloc(sizeof(char *) * 10);
        size_t count = 0, capacity = 10;

        if (read_directory(dir, &filenames, &count, &capacity) == -1)
        {
            closedir(dir);
            free(filenames);
            return (-1);
        }

        closedir(dir);
        sort_filenames(filenames, count, options);
        print_directory_contents(dir_name, filenames, count, options);
    }
    else
    {
        /* It's not a directory, it's a file */
        printf("%s\n", dir_name);
    }

    return (0);
}

/**
 * process_arguments - Processes the command line arguments.
 * @argc: The number of arguments.
 * @argv: The array of arguments.
 * @options: Options bitmask.
 *
 * Return: 0 if success, EXIT_FAILURE if no directories found.
 */
int process_arguments(int argc, char *argv[], int options)
{
    int no_dir_found = 0;

    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            if (process_directory(argv[i], options) == -1)
            {
                no_dir_found = 1;
            }
        }
    }

    return (no_dir_found ? EXIT_FAILURE : 0);
}
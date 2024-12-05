#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "hls.h"
#include "utils.h"

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
 */
void print_error(const char *prog_name, const char *dir)
{
    fprintf(stderr, "%s: cannot access %s: %s\n", prog_name, dir, strerror_custom(errno));
}

/**
 * print_directory_contents - Prints the directory contents.
 * @dir_name: The name of the directory.
 * @filenames: Array of filenames.
 * @count: The number of filenames.
 * @is_multiple_dirs: Flag to indicate if multiple directories are being processed.
 */
void print_directory_contents(const char *dir_name, char **filenames, size_t count, int is_multiple_dirs)
{
    if (is_multiple_dirs)
    {
        printf("%s:\n", dir_name);  /* Print the directory name only if multiple directories are handled */
    }

    for (size_t j = 0; j < count; j++)
    {
        printf("%s\n", filenames[j]);
        free(filenames[j]);
    }
    free(filenames);
}

/**
 * process_directory - Processes a directory and lists its contents.
 * @dir_name: The directory name.
 * @is_multiple_dirs: Flag to indicate if multiple directories are being processed.
 *
 * Return: 0 if success, -1 if error.
 */
int process_directory(const char *dir_name, int is_multiple_dirs)
{
    struct stat statbuf;

    if (stat(dir_name, &statbuf) == -1)
    {
        print_error("./hls_01", dir_name);
        return (-1);
    }

    if (S_ISDIR(statbuf.st_mode))
    {
        DIR *dir = opendir(dir_name);

        if (!dir)
        {
            print_error("./hls_01", dir_name);
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
        sort_filenames(filenames, count);
        print_directory_contents(dir_name, filenames, count, is_multiple_dirs);
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
 *
 * Return: 0 if success, EXIT_FAILURE if no directories found.
 */
int process_arguments(int argc, char *argv[])
{
    int no_dir_found = 0;
    int is_multiple_dirs = (argc > 2);

    for (int i = 1; i < argc; i++)
    {
        if (process_directory(argv[i], is_multiple_dirs) == -1)
        {
            no_dir_found = 1;
        }
    }

    return (no_dir_found ? EXIT_FAILURE : 0);
}
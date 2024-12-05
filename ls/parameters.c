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
 * @is_multiple_dirs: Flag indicating if multiple directories are being processed.
 */
void print_directory_contents(const char *dir_name, char **filenames, size_t count, int options, int is_multiple_dirs)
{
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
 * @is_multiple_dirs: Flag indicating if multiple directories are being processed.
 *
 * Return: 0 if success, -1 if error.
 */
int process_directory(const char *dir_name, int options, int is_multiple_dirs)
{
    struct stat statbuf;

    if (stat(dir_name, &statbuf) == -1)
    {
        print_error("./hls_02", dir_name, 0);
        return (-1);
    }

    if (S_ISDIR(statbuf.st_mode))
    {
        DIR *dir = opendir(dir_name);

        if (!dir)
        {
            print_error("./hls_02", dir_name, 1);
            return (-1);
        }

        struct dirent *entry;
        char **filenames = malloc(sizeof(char *) * 10);
        size_t count = 0, capacity = 10;

        while ((entry = readdir(dir)) != NULL)
        {
            // Skip hidden files
            if (entry->d_name[0] == '.')
            {
                continue;
            }

            if (count >= capacity)
            {
                capacity *= 2;
                filenames = realloc(filenames, sizeof(char *) * capacity);
            }
            filenames[count] = my_strdup(entry->d_name);
            count++;
        }

        closedir(dir);

        if (count == 0)
        {
            free(filenames);
            filenames = NULL;
        }

        sort_filenames(filenames, count, options);
        print_directory_contents(dir_name, filenames, count, options, is_multiple_dirs);
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
    int is_multiple_dirs = 0;
    int dir_count = 0;
    int file_count = 0;

    // Count the number of directories and files
    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            struct stat statbuf;
            if (stat(argv[i], &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
            {
                dir_count++;
            }
            else
            {
                file_count++;
            }
        }
    }

    is_multiple_dirs = (dir_count > 1);

    // Print files first
    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            struct stat statbuf;
            if (stat(argv[i], &statbuf) == 0 && !S_ISDIR(statbuf.st_mode))
            {
                if (process_directory(argv[i], options, is_multiple_dirs) == -1)
                {
                    no_dir_found = 1;
                }
            }
        }
    }

    // Print directories after files
    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            struct stat statbuf;
            if (stat(argv[i], &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
            {
                if (is_multiple_dirs)
                {
                    printf("%s:\n", argv[i]);
                }
                if (process_directory(argv[i], options, is_multiple_dirs) == -1)
                {
                    no_dir_found = 1;
                }
                if (is_multiple_dirs && i < argc - 1)
                {
                    printf("\n");
                }
            }
        }
    }

    return (no_dir_found ? EXIT_FAILURE : 0);
}
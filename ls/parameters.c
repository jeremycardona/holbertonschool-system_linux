#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
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
 * print_file_info - Prints detailed information about a file.
 * @dir_name: The directory name.
 * @file_name: The file name.
 */
void print_file_info(const char *dir_name, const char *file_name)
{
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dir_name, file_name);

    struct stat statbuf;
    if (lstat(path, &statbuf) == -1)
    {
        perror("lstat");
        return;
    }

    // Print file type and permissions
    printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
    printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
    printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
    printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
    printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
    printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
    printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
    printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
    printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
    printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

    // Print number of links
    printf(" %lu", statbuf.st_nlink);

    // Print owner and group
    struct passwd *pw = getpwuid(statbuf.st_uid);
    struct group *gr = getgrgid(statbuf.st_gid);
    printf(" %s %s", pw->pw_name, gr->gr_name);

    // Print size
    printf(" %5ld", statbuf.st_size);

    // Print modification time
    char timebuf[80];
    struct tm *tm_info = localtime(&statbuf.st_mtime);
    time_t current_time = time(NULL);
    double diff_time = difftime(current_time, statbuf.st_mtime);

    if (diff_time < 6 * 30 * 24 * 60 * 60) // Less than 6 months
    {
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
    }
    else
    {
        strftime(timebuf, sizeof(timebuf), "%b %d %Y", tm_info);
    }
    printf(" %s", timebuf);

    // Print file name
    printf(" %s\n", file_name);
}

/**
 * print_directory_contents - Prints the directory contents.
 * @dir_name: The name of the directory.
 * @filenames: Array of filenames.
 * @count: The number of filenames.
 * @options: Options bitmask.
 * @is_multiple_dirs: Flag indicating if multiple directories are being processed.
 */
void print_directory_contents(const char *dir_name, char **filenames, size_t count, int options)
{
    for (size_t j = 0; j < count; j++)
    {
        if (options & OPT_LONG_FORMAT)
        {
            print_file_info(dir_name, filenames[j]);
        }
        else
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
        }
        free(filenames[j]);
    }
    free(filenames);

    if (!(options & OPT_ONE_PER_LINE) && !(options & OPT_LONG_FORMAT))
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
int process_directory(char *prog_name, const char *dir_name, int options)
{
    struct stat statbuf;

    if (lstat(dir_name, &statbuf) == -1)
    {
        print_error(prog_name, dir_name, 0);
        return (-1);
    }

    if (S_ISDIR(statbuf.st_mode))
    {
        DIR *dir = opendir(dir_name);

        if (!dir)
        {
            print_error(prog_name, dir_name, 1);
            return (-1);
        }

        struct dirent *entry;
        char **filenames = malloc(sizeof(char *) * 10);
        size_t count = 0, capacity = 10;

        while ((entry = readdir(dir)) != NULL)
        {
            // Skip hidden files unless -a or -A option is specified
            if (!(options & OPT_ALL) && !(options & OPT_ALMOST_ALL) && entry->d_name[0] == '.')
            {
                continue;
            }

            // Skip . and .. if -A option is specified
            if ((options & OPT_ALMOST_ALL) && (my_strcmp(entry->d_name, ".") == 0 || my_strcmp(entry->d_name, "..") == 0))
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
    int is_multiple_dirs = 0;
    int dir_count = 0;
    int file_start_index = 1; // Start from 1 to skip the program name

    // Count the number of directories/files
    for (int i = file_start_index; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            dir_count++;
        }
    }

    is_multiple_dirs = (dir_count > 1);

    // First, process non-directory files
    for (int i = file_start_index; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            struct stat statbuf;
            if (lstat(argv[i], &statbuf) == -1)
            {
                print_error(argv[0], argv[i], 0);
                no_dir_found = 1;
                continue;
            }

            if (!S_ISDIR(statbuf.st_mode))
            {
                printf("%s\n", argv[i]);
            }
        }
    }

    // Then, process directories
    for (int i = file_start_index; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            struct stat statbuf;
            if (lstat(argv[i], &statbuf) == -1)
            {
                continue; // Already handled in the previous loop
            }

            if (S_ISDIR(statbuf.st_mode))
            {
                if (is_multiple_dirs)
                {
                    printf("%s:\n", argv[i]);
                }
                if (process_directory(argv[0], argv[i], options) == -1)
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
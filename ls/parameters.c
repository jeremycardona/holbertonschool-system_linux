#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "hls.h"
#include "utils.h"

/**
 * strerror_custom - custom strerror function to handle errors.
 * @errnum: the error number.
 *
 * Return: a string corresponding to the error.
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
 * print_error - prints an error message.
 * @prog_name: name of the program.
 * @dir: the directory or file causing the error.
 */
void print_error(const char *prog_name, const char *dir)
{
	fprintf(stderr, "./%s: cannot access %s: %s\n", prog_name, dir,
			strerror_custom(errno));
}

/**
 * print_directory_contents - prints the directory contents.
 * @filenames: array of filenames.
 * @count: number of filenames.
 */
void print_directory_contents(char **filenames, size_t count)
{
	for (size_t j = 0; j < count; j++)
	{
		printf("%s\n", filenames[j]);
		free(filenames[j]);
	}
	free(filenames);
}

/**
 * process_directory - processes a directory and lists its contents.
 * @dir_name: the directory name.
 *
 * Return: 0 if success, -1 if error.
 */
int process_directory(const char *dir_name)
{
	struct stat statbuf;

	if (stat(dir_name, &statbuf) == -1)
	{
		print_error("hls_01", dir_name);
		return (-1);
	}

	if (S_ISDIR(statbuf.st_mode))
	{
		DIR *dir = opendir(dir_name);

		if (!dir)
		{
			print_error("hls_01", dir_name);
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
		print_directory_contents(filenames, count);
	}
	else
	{
		/* It's not a directory, it's a file */
		printf("%s\n", dir_name);
	}

	return (0);
}

/**
 * process_arguments - processes the command line arguments.
 * @argc: number of arguments.
 * @argv: array of arguments.
 *
 * Return: 0 if success, EXIT_FAILURE if no directories found.
 */
int process_arguments(int argc, char *argv[])
{
	int no_dir_found = 0;

	for (int i = 1; i < argc; i++)
	{
		if (process_directory(argv[i]) == -1)
		{
			no_dir_found = 1;
		}
	}

	return (no_dir_found ? EXIT_FAILURE : 0);
}

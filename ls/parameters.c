#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "utils.h"
#include "hls.h"

/**
 * print_error - Prints error message when directory cannot be accessed.
 * @prog_name: The program name.
 * @dir: The directory/file name.
 */
void print_error(const char *prog_name, const char *dir)
{
	/* Ensure both prog_name and dir are strings */
	fprintf(stderr, "%s: cannot access '%s': %s\n",
		prog_name, dir, strerror(errno));
}

/**
 * print_directory_contents - Sort and print the contents of a directory.
 * @dir_name: The name of the directory.
 * @filenames: List of filenames in the directory.
 * @count: The number of filenames.
 */
void print_directory_contents(const char *dir_name,
	char **filenames, size_t count)
{
	printf("%s:\n", dir_name);
	for (size_t j = 0; j < count; j++)
	{
		printf("%s\n", filenames[j]);
		free(filenames[j]);
	}
	free(filenames);
}

/**
 * process_directory - Process and list files in a directory.
 * @dir_name: The name of the directory to process.
 * Return: 0 on success, -1 on failure.
 */
int process_directory(const char *dir_name)
{
	DIR *dir = opendir(dir_name);

	if (!dir)
	{
		print_error(dir_name, dir_name);
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
	print_directory_contents(dir_name, filenames, count);

	return (0);
}

/**
 * process_arguments - Process each directory or file passed as an argument.
 * @argc: Argument count.
 * @argv: Argument vector.
 *
 * Description: Process and list files in the directories/files.
 * Return: Always 0.
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

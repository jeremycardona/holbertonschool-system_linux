#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "utils.h"
#include "hls.h"

/**
 * main - List and sort the contents of the current directory.
 *
 * Description: Program that lists the contents of the current
 * directory in alphabetical order.
 * Return: Always 0.
 */
int main(void)
{
	DIR *dir = opendir(".");
	char **filenames = malloc(sizeof(char *) * 10);
	size_t count = 0, capacity = 10;

	if (!dir)
	{
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	if (read_directory(dir, &filenames, &count, &capacity) == -1)
	{
		closedir(dir);
		exit(EXIT_FAILURE);
	}

	closedir(dir);

	sort_filenames(filenames, count);

	for (size_t i = 0; i < count; i++)
	{
		printf("%s\n", filenames[i]);
		free(filenames[i]);
	}

	free(filenames);
	return (0);
}

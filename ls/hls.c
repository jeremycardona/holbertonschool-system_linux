#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "hls.h"
#include "utils.h"


/**
 * read_directory - Reads the entries of a directory and stores filenames.
 * @dir: The directory stream to read from.
 * @filenames: Pointer to the array to store filenames.
 * @count: Pointer to the current count of filenames.
 * @capacity: Pointer to the current capacity of the filenames array.
 *
 * Return: 0 if successful, otherwise exit with failure.
 */
int read_directory(DIR *dir, char ***filenames, size_t *count,
	size_t *capacity)
{
	struct dirent *entry;

	while ((entry = readdir(dir)))
	{
		if (entry->d_name[0] != '.')
		{
			if (*count == *capacity)
			{
				if (resize_filenames(filenames, count, capacity) == -1)
					return (-1);
			}

			(*filenames)[*count] = malloc(my_strlen(entry->d_name) + 1);
			if (!(*filenames)[*count])
			{
				perror("malloc");
				return (-1);
			}
			my_strcpy((*filenames)[*count], entry->d_name);
			(*count)++;
		}
	}

	return (0);
}

/**
 * resize_filenames - Resizes the filenames array when needed.
 * @filenames: Pointer to the filenames array.
 * @count: Pointer to the current count of filenames.
 * @capacity: Pointer to the current capacity of the filenames array.
 *
 * Return: 0 if successful, otherwise -1 for failure.
 */
int resize_filenames(char ***filenames, size_t *count, size_t *capacity)
{
	size_t new_capacity = *capacity * 2;
	char **new_filenames = malloc(sizeof(char *) * new_capacity);

	if (!new_filenames)
	{
		perror("malloc");
		return (-1);
	}

	for (size_t i = 0; i < *count; i++)
		new_filenames[i] = (*filenames)[i];

	free(*filenames);
	*filenames = new_filenames;
	*capacity = new_capacity;

	return (0);
}

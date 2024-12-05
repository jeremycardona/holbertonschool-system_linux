#include "utils.h"

/**
 * my_strlen - Custom string length function.
 * @str: The input string.
 * Return: Length of the string.
 */
size_t my_strlen(const char *str)
{
	size_t length = 0;

	while (str[length])
	{
		length++;
	}

	return (length);
}

/**
 * my_strcpy - Custom string copy function.
 * @dest: Destination buffer.
 * @src: Source string.
 * Return: Pointer to the destination string.
 */
char *my_strcpy(char *dest, const char *src)
{
	char *start = dest;

	while ((*dest++ = *src++))
		;

	return (start);
}

/**
 * my_tolower - Converts a character to lowercase.
 * @c: The character to convert.
 * Return: The lowercase equivalent if the character is uppercase,
 * otherwise returns the character itself.
 */
char my_tolower(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return (c + ('a' - 'A'));
	}
	return (c);
}

/**
 * my_strcmp - Custom string comparison function (case-insensitive).
 * @str1: First string.
 * @str2: Second string.
 * Return: <0 if str1 < str2, 0 if str1 == str2, >0 if str1 > str2.
 */
int my_strcmp(const char *str1, const char *str2)
{
	while (*str1 && *str2)
	{
		char c1 = my_tolower(*str1);
		char c2 = my_tolower(*str2);

		if (c1 != c2)
		{
			return (c1 - c2);
		}

		str1++;
		str2++;
	}

	return (my_tolower(*str1) - my_tolower(*str2));
}

/**
 * sort_filenames - Sorts an array of filenames in alphabetical order.
 * @filenames: Array of filenames.
 * @count: Number of filenames in the array.
 */
void sort_filenames(char **filenames, size_t count)
{
	size_t i, j;

	for (i = 0; i < count - 1; i++)
	{
		for (j = i + 1; j < count; j++)
		{
			if (my_strcmp(filenames[i], filenames[j]) > 0)
			{
				char *temp = filenames[i];

				filenames[i] = filenames[j];
				filenames[j] = temp;
			}
		}
	}
}

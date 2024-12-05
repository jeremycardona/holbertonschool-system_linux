#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "options.h"

static int options_global;

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
 * Return: Pointer to the destination buffer.
 */
char *my_strcpy(char *dest, const char *src)
{
    char *ptr = dest;

    while (*src)
    {
        *ptr++ = *src++;
    }
    *ptr = '\0';

    return (dest);
}
/**
 * my_strcmp - Compares two strings.
 * @str1: The first string.
 * @str2: The second string.
 *
 * Return: An integer less than, equal to, or greater than zero if str1 is found,
 * respectively, to be less than, to match, or be greater than str2.
 */
int my_strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}
/**
 * my_tolower - Custom tolower function.
 * @c: The input character.
 * Return: Lowercase character.
 */
char my_tolower(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return (c + 32);
    }
    return (c);
}

/**
 * my_strcasecmp - Custom case-insensitive string compare function.
 * @str1: First string.
 * @str2: Second string.
 * Return: Comparison result.
 */
int my_strcasecmp(const char *str1, const char *str2)
{
    while (*str1 && (my_tolower(*str1) == my_tolower(*str2)))
    {
        str1++;
        str2++;
    }
    return (my_tolower(*(unsigned char *)str1) - my_tolower(*(unsigned char *)str2));
}

/**
 * my_strdup - Custom strdup function.
 * @str: The input string.
 * Return: Pointer to the duplicated string.
 */
char *my_strdup(const char *str)
{
    size_t len = my_strlen(str) + 1;
    char *copy = malloc(len);
    if (copy)
    {
        my_strcpy(copy, str);
    }
    return copy;
}

/**
 * compare_filenames - Comparison function for filenames.
 * @a: First filename.
 * @b: Second filename.
 * Return: Comparison result.
 */
int compare_filenames(const void *a, const void *b)
{
    const char **filename1 = (const char **)a;
    const char **filename2 = (const char **)b;

    if (options_global & OPT_SORT_SIZE)
    {
        struct stat stat1, stat2;
        stat(*filename1, &stat1);
        stat(*filename2, &stat2);
        return (stat2.st_size - stat1.st_size);
    }
    else if (options_global & OPT_SORT_TIME)
    {
        struct stat stat1, stat2;
        stat(*filename1, &stat1);
        stat(*filename2, &stat2);
        return (stat2.st_mtime - stat1.st_mtime);
    }
    else
    {
        return my_strcasecmp(*filename1, *filename2);
    }
}

/**
 * swap - Swaps two elements in an array.
 * @a: First element.
 * @b: Second element.
 */
void swap(char **a, char **b)
{
    char *temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * bubble_sort - Sorts an array using bubble sort.
 * @array: Array to sort.
 * @size: Size of the array.
 */
void bubble_sort(char **array, size_t size)
{
    for (size_t i = 0; i < size - 1; i++)
    {
        for (size_t j = 0; j < size - i - 1; j++)
        {
            if (compare_filenames(&array[j], &array[j + 1]) > 0)
            {
                swap(&array[j], &array[j + 1]);
            }
        }
    }
}

/**
 * sort_filenames - Sorts the filenames array.
 * @filenames: Array of filenames.
 * @count: The number of filenames.
 * @options: Options bitmask.
 */
void sort_filenames(char **filenames, size_t count, int options)
{
    options_global = options;
    bubble_sort(filenames, count);

    if (options & OPT_REVERSE)
    {
        for (size_t i = 0; i < count / 2; i++)
        {
            swap(&filenames[i], &filenames[count - i - 1]);
        }
    }
}
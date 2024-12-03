#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/** 
 *  swap - swaps two strings in the array.
 *  @a: pointer to the first string
 *  @b: pointer to the second string
 */
void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

/** 
 *  sort - sorts the array of strings using bubble sort.
 *  @arr: array of strings
 *  @n: number of elements in the array
 */
void sort(char *arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j], arr[j + 1]) > 0) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

/** 
 *  main - lists the contents of the current directory excluding hidden files, sorted.
 *
 *  Return: 0 on success, 1 on failure.
 */
int main(void) {
    DIR *dir = opendir(".");
    struct dirent *entry;
    char *filenames[1024];  // Array to hold file names
    int count = 0;
    int i;

    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    // Read entries into array
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {  // Skip hidden files
            filenames[count] = malloc(strlen(entry->d_name) + 1);  // Allocate memory for the name
            if (filenames[count] == NULL) {
                perror("malloc");
                closedir(dir);
                return 1;
            }
            strcpy(filenames[count], entry->d_name);  // Copy the name
            count++;
        }
    }
    closedir(dir);

    // Sort filenames
    sort(filenames, count);

    // Print sorted list
    for (i = 0; i < count; i++) {
        printf("%s\n", filenames[i]);
        free(filenames[i]);  // Free allocated memory
    }

    return 0;
}

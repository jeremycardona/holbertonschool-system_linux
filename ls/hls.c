#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

/** 
 *  compare - comparison function for qsort
 *  @a: pointer to first element
 *  @b: pointer to second element
 *
 *  Return: negative, zero, or positive depending on lexicographical order.
 */
int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

/** 
 *  main - list the contents of the current directory excluding hidden files, sorted.
 *
 *  Description: Program that lists the contents of the current directory,
 *  excluding hidden files and folders, in sorted order.
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
        if (entry->d_name[0] != '.') {
            filenames[count] = strdup(entry->d_name);  // Duplicate and store the name
            if (filenames[count] == NULL) {
                perror("strdup");
                closedir(dir);
                return 1;
            }
            count++;
        }
    }
    closedir(dir);

    // Sort filenames
    qsort(filenames, count, sizeof(char *), compare);

    // Print sorted list
    for (i = 0; i < count; i++) {
        printf("%s\n", filenames[i]);
        free(filenames[i]);  // Free allocated memory
    }

    return 0;
}

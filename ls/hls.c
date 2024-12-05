#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "utils.h"

/**
 * main - List and sort the contents of the current directory.
 *
 * Description: Program that lists the contents of the current
 * directory in alphabetical order.
 * Return: 0 exit.
 */
int main(void) {
    DIR *dir = opendir(".");
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    char **filenames = NULL;
    size_t count = 0;

    // Read directory entries into an array
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] != '.') {
            filenames = realloc(filenames, sizeof(char *) * (count + 1));
            if (!filenames) {
                perror("realloc");
                closedir(dir);
                exit(EXIT_FAILURE);
            }
            filenames[count] = malloc(my_strlen(entry->d_name) + 1);
            if (!filenames[count]) {
                perror("malloc");
                closedir(dir);
                exit(EXIT_FAILURE);
            }
            my_strcpy(filenames[count], entry->d_name);
            count++;
        }
    }

    closedir(dir);

    // Sort the filenames array
    sort_filenames(filenames, count);

    // Print sorted filenames
    for (size_t i = 0; i < count; i++) {
        printf("%s\n", filenames[i]);
        free(filenames[i]);
    }

    free(filenames);
    return 0;
}

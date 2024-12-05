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
    char **filenames = malloc(sizeof(char *) * 10);  // Initial allocation for 10 filenames
    size_t count = 0;
    size_t capacity = 10;

    // Read directory entries into an array
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] != '.') {
            // Check if resizing is needed
            if (count == capacity) {
                // Allocate a larger block with double capacity
                size_t new_capacity = capacity * 2;
                char **new_filenames = malloc(sizeof(char *) * new_capacity);
                if (!new_filenames) {
                    perror("malloc");
                    closedir(dir);
                    exit(EXIT_FAILURE);
                }

                // Copy old data to new block
                for (size_t i = 0; i < count; i++) {
                    new_filenames[i] = filenames[i];
                }
                free(filenames);
                filenames = new_filenames;
                capacity = new_capacity;
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

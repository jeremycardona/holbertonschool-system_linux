#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Comparison function for qsort (case-sensitive)
int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(void) {
    DIR *dir = opendir(".");
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *read;
    char *filenames[256]; // Array to store file names (assuming max 256 files)
    int count = 0;

    // Read directory entries
    while ((read = readdir(dir))) {
        if (read->d_name[0] != '.') {
            // Allocate memory for file name and copy
            filenames[count] = malloc(strlen(read->d_name) + 1);
            if (!filenames[count]) {
                perror("malloc");
                closedir(dir);
                exit(EXIT_FAILURE);
            }
            strcpy(filenames[count], read->d_name);
            count++;
        }
    }
    closedir(dir);

    // Sort the file names
    qsort(filenames, count, sizeof(char *), compare);

    // Print sorted file names
    for (int i = 0; i < count; i++) {
        printf("%s\n", filenames[i]);
        free(filenames[i]); // Free allocated memory
    }

    return 0;
}

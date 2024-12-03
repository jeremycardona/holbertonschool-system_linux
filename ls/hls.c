#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Bubble sort comparison function (case-sensitive)
void bubbleSort(char *arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j], arr[j + 1]) > 0) {
                // Swap the elements
                char *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
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

    // Sort the file names using bubble sort
    bubbleSort(filenames, count);

    // Print sorted file names
    for (int i = 0; i < count; i++) {
        printf("%s\n", filenames[i]);
        free(filenames[i]); // Free allocated memory
    }

    return 0;
}

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

int main(void){
    DIR *dir = opendir(".");
    struct dirent *read;

    while ((read = readdir(dir))){
        // Print file name
        printf("%s\n", read->d_name);
    }

    closedir(dir);
    return 0;
}
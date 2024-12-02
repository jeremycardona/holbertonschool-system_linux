#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char **argv){
    DIR *dir;

    dir = opendir("test");

    struct dirent *read;

    while ((read = readdir(dir)) != NULL)
        printf("%s\n", read->d_name);
    
    closedir(dir);
    return 0;
}
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

/** 
 *  main - list the contents for current directory
 *
 *  Description: program that lists the contents
 *  of the current directory.
 *  Return: 0 exit
 */
int main(void){
    DIR *dir = opendir(".");
    struct dirent *read;

    while ((read = readdir(dir))){
        // Print file name
        if (read->d_name[0] != '.'){
            printf("%s\n", read->d_name);
        }
    }

    closedir(dir);
    return 0;
}
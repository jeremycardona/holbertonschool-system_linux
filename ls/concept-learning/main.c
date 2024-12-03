#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void print_file_type(mode_t mode) {
    if (S_ISREG(mode)) printf("-");
    else if (S_ISDIR(mode)) printf("d");
    else if (S_ISCHR(mode)) printf("c");
    else if (S_ISBLK(mode)) printf("b");
    else if (S_ISFIFO(mode)) printf("p");
    else if (S_ISLNK(mode)) printf("l");
    else if (S_ISSOCK(mode)) printf("s");
    else printf("?");
}

void print_permissions(mode_t mode) {
    printf("%c", (mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (mode & S_IROTH) ? 'r' : '-');
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');
    printf("%c ", (mode & S_IXOTH) ? 'x' : '-');
}

int main(int argc, char **argv) {
    DIR *dir;
    struct dirent *read;
    struct stat buf;
    char filepath[1024];

    // Open the directory passed as an argument
    if (argc >= 2) {
        dir = opendir(argv[1]);
    } else {
        printf("You forgot to pass a directory as an argument.\n");
        return -1;
    }

    if (!dir) {
        printf("Invalid directory.\n");
        return -1;
    }

    // Read and process each entry in the directory
    while ((read = readdir(dir)) != NULL) {
        snprintf(filepath, sizeof(filepath), "%s/%s", argv[1], read->d_name); 

        if (lstat(filepath, &buf) == 0) {
            // get user id
            struct passwd *usr = getpwuid(buf.st_uid);
            // get group id
            struct group *grp = getgrgid(buf.st_gid);

            // Print file type
            print_file_type(buf.st_mode);

            // Print permissions
            print_permissions(buf.st_mode);
            //print group user id and group id
            printf("%s %s ", usr->pw_name, grp->gr_name);
            // Print size and modification time
            printf("%ld ", buf.st_size);
            char *mod_time = ctime(&buf.st_mtime);
            mod_time[strlen(mod_time) - 1] = '\0'; // Remove newline
            printf("%s ", mod_time);

            // Print file name
            printf("%s\n", read->d_name);
        } else {
            perror("lstat failed");
        }
    }

    closedir(dir);
    return 0;
}

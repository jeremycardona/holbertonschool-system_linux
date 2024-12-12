#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    printf("Enter a line of text:\n");
    nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
        perror("getline");
        free(line);
        return (1);
    }

    printf("You entered: %s", line);
    free(line);

    return (0);
}
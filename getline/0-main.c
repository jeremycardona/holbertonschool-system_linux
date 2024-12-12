#include "laps.h"

/**
 * main - entry point.
 *
 * Return: always 0.
 */
int main()
{

    int ids1[3] = {1, 42, 101};
    int ids2[1] = {11};
    int ids3[5] = {1, 42, 3, 11, 2};

    race_state(ids1, 3);
    printf("--\n");
    race_state(ids1, 3);
    printf("--\n");
    race_state(ids1, 3);
    printf("--\n");
    race_state(ids2, 1);
    printf("--\n");
    race_state(ids1, 3);
    printf("--\n");
    race_state(ids2, 1);
    printf("--\n");
    race_state(ids1, 3);
    printf("--\n");
    race_state(ids2, 1);
    printf("--\n");
    race_state(ids1, 3);
    printf("--\n");
    race_state(ids2, 1);
    printf("--\n");  
    race_state(ids3, 5);
    
    printf("--\n");
    race_state(ids3, 5);
    
    printf("--\n");
    race_state(ids3, 5);
    
    printf("--\n");
    race_state(NULL, 0);
    return (0);
}
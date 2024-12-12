#ifndef LAPS_H
#define LAPS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Car
{
    int id;
    int laps;
    struct Car *next;
} Car;

void race_state(int *id, size_t size);
void printCars(void);
void freeCars(void);
int createCar(int id);
void updateLaps(int id);
void checkNewCars(int *id, size_t size);
int carExists(int id);
void updateLapsForCars(int *id, size_t size);

#endif /* LAPS_H */

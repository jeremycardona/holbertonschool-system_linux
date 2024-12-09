#ifndef LAPS_H
#define LAPS_H

#include <stdio.h>
#include <stdlib.h>

/**
 * struct car - car struct
 * @id: car id
 * @laps: number of laps
 * @next: pointer to next car
 */
typedef struct car
{
	int id;
	int laps;
	struct car *next;
} Car;

void race_state(int *id, size_t size);
int createCar(int id);
void freeCars(void);
void updateLaps(int id);
void printCars(void);

#endif /* LAPS_H */
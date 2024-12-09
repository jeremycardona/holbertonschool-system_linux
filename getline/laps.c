#include "laps.h"

/* Static variable for managing cars in the race */
static Car *cars;

/**
 * race_state - Keep track of the number of laps made by several cars.
 * @id: Array of car identifiers.
 * @size: Size of the id array.
 */
void race_state(int *id, size_t size)
{
	size_t i;
	Car *tmp;
	int exists;

	if (id == NULL || size == 0)
	{
		printCars();
		freeCars();
		return;
	}
	for (i = 0; i < size; i++)
	{
		tmp = cars;
		exists = 0;

		/* Check if the car already exists */
		while (tmp)
		{
			if (tmp->id == id[i])
			{
				exists = 1;
				break;
			}
			tmp = tmp->next;
		}
		if (!exists)
		{
			if (createCar(id[i]) == 1)
			{
				freeCars();
				return;
			}
			printf("Car %d joined the race\n", id[i]);
		}
	}
	for (i = 0; i < size; i++)
		updateLaps(id[i]);

	printCars();
}

/**
 * createCar - Create a new car.
 * @id: Car identifier.
 *
 * Return: 0 on success, 1 on failure.
 */
int createCar(int id)
{
	Car *newCar, *current;

	newCar = malloc(sizeof(Car));
	if (newCar == NULL)
		return (1);

	newCar->id = id;
	newCar->laps = 0;
	newCar->next = NULL;

	if (cars == NULL || cars->id > id)
	{
		newCar->next = cars;
		cars = newCar;
	}
	else
	{
		current = cars;
		while (current->next != NULL && current->next->id < id)
			current = current->next;

		newCar->next = current->next;
		current->next = newCar;
	}

	return (0);
}

/**
 * updateLaps - Increment the lap count for a car.
 * @id: Car identifier.
 */
void updateLaps(int id)
{
	Car *tmp;

	tmp = cars;

	while (tmp)
	{
		if (tmp->id == id)
		{
			tmp->laps++;
			break;
		}
		tmp = tmp->next;
	}
}

/**
 * freeCars - Free all car memory.
 */
void freeCars(void)
{
	Car *tmp;

	while (cars)
	{
		tmp = cars;
		cars = cars->next;
		free(tmp);
	}
}

/**
 * printCars - Print the current race state.
 */
void printCars(void)
{
	Car *tmp;

	printf("Race state:\n");

	tmp = cars;
	while (tmp)
	{
		printf("Car %d [%d laps]\n", tmp->id, tmp->laps);
		tmp = tmp->next;
	}
}

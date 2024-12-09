#include "laps.h"

static Car *cars;

/**
 * race_state - keep track of the number of laps made by several cars
 * @id: array of car identifiers
 * @size: size of the id array
 */
void race_state(int *id, size_t size)
{
	size_t i;

	if (id == NULL || size == 0)
	{
		printCars();
		freeCars();
		return;
	}

	for (i = 0; i < size; i++)
	{
		if (!carExists(id[i]))
		{
			if (createCar(id[i]) == 1)
			{
				freeCars();
				return;
			}
			printf("Car %d joined the race\n", id[i]);
		}
	}

	printCars();

	for (i = 0; i < size; i++)
	{
		updateLaps(id[i]);
	}
}

/**
 * carExists - check if a car with the given id exists
 * @id: car id
 *
 * Return: 1 if car exists, 0 otherwise
 */
int carExists(int id)
{
	Car *tmp = cars;

	while (tmp)
	{
		if (tmp->id == id)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

/**
 * createCar - create a new car
 * @id: car id
 *
 * Return: 0 on success, 1 on failure
 */
int createCar(int id)
{
	Car *new, *current, *prev;

	new = malloc(sizeof(Car));
	if (new == NULL)
		return (1);

	new->id = id;
	new->laps = 0;
	new->next = NULL;

	if (cars == NULL || cars->id > id)
	{
		new->next = cars;
		cars = new;
	}
	else
	{
		current = cars;
		prev = NULL;
		while (current != NULL && current->id < id)
		{
			prev = current;
			current = current->next;
		}
		new->next = current;
		if (prev != NULL)
			prev->next = new;
	}
	return (0);
}

/**
 * freeCars - free all cars
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
 * updateLaps - update the number of laps of a car
 * @id: car id
 */
void updateLaps(int id)
{
	Car *tmp = cars;

	while (tmp)
	{
		if (tmp->id == id)
		{
			tmp->laps += 1;
			break;
		}
		tmp = tmp->next;
	}
}

/**
 * printCars - print all cars
 */
void printCars(void)
{
	Car *tmp = cars;

	printf("Race state:\n");
	while (tmp)
	{
		printf("Car %d [%d laps]\n", tmp->id, tmp->laps);
		tmp = tmp->next;
	}
}

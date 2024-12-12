#include "laps.h"

static Car *cars;

/**
 * race_state - Keep track of the number of laps made by several cars.
 * @id: Array of car identifiers.
 * @size: Size of the id array.
 */
void race_state(int *id, size_t size)
{
    if (id == NULL || size == 0)
    {
        freeCars();
        return;
    }

    // Check for new cars and add them
    checkNewCars(id, size);

    // Update laps for existing cars (cars with 0 laps initially)
    updateLapsForCars(id, size);

    // Print race state after all updates
    printCars();
}

/**
 * checkNewCars - Check for new car ids and add them to the race.
 * @id: Array of car identifiers.
 * @size: Size of the id array.
 */
void checkNewCars(int *id, size_t size)
{
    size_t i;

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
}

/**
 * carExists - Check if a car with the given id exists.
 * @id: Car identifier.
 *
 * Return: 1 if car exists, 0 otherwise.
 */
int carExists(int id)
{
    Car *tmp = cars;

    while (tmp)
    {
        if (tmp->id == id)
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

/**
 * createCar - Create a new car.
 * @id: Car identifier.
 *
 * Return: 0 on success, 1 on failure.
 */
int createCar(int id)
{
    Car *new, *current, *prev;

    new = malloc(sizeof(Car));
    if (new == NULL)
        return (1);
    new->id = id;
    new->laps = 0;  // Initialize laps to 0 when the car is created
    new->next = NULL;

    // Insert car in sorted order
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
 * freeCars - Free all cars.
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
 * updateLapsForCars - Update the number of laps for each car in the id array.
 * @id: Array of car identifiers.
 * @size: Size of the id array.
 */
void updateLapsForCars(int *id, size_t size)
{
    size_t i;

    for (i = 0; i < size; i++)
    {
        updateLaps(id[i]);
    }
}

/**
 * updateLaps - Update the number of laps of a car.
 * @id: Car identifier.
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
 * printCars - Print all cars.
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

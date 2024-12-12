#ifndef LAPS_H
#define LAPS_H

#include <stdio.h>
#include <stdlib.h>

/**
 * struct car_node - A node representing a car
 * @id: Car identifier
 * @lap_count: Number of laps completed
 * @next: Pointer to the next car
 *
 * Description: This structure represents
 * a node in a linked list of cars.
 * It contains the car identifier, the number
 * of laps completed, and a pointer to the next car.
 */

typedef struct car_node
{
	int id;				   /* Car identifier */
	size_t lap_count;	   /* Number of laps completed */
	struct car_node *next; /* Pointer to the next car */
} car_node;

/**
 * print_race_state - Prints the state of the race.
 * @head: Pointer to the head node of the linked list.
 */
void print_race_state(car_node **head);

/**
 * insert_sorted_car - Inserts a car node in the linked list in sorted order.
 * @head: Pointer to the head node of the linked list.
 * @new_car: Pointer to the new car node.
 */
void insert_sorted_car(car_node **head, car_node *new_car);

/**
 * create_car_node - Creates a new car node with the given id.
 * @id: Car identifier.
 *
 * Return: A new car node.
 */
car_node *create_car_node(int id);

/**
 * check_new_cars - Checks for new cars and updates the race state.
 * @head: Pointer to the head node of the linked list.
 * @id_array: Array of car identifiers.
 * @array_size: Size of the id array.
 */
void check_new_cars(car_node **head, int *id_array, size_t array_size);

/**
 * race_state - Tracks the number of laps made by cars in the race.
 * @id_array: Array of car identifiers.
 * @array_size: Size of the id array.
 */
void race_state(int *id_array, size_t array_size);

#endif /* LAPS_H */

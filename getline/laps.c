#include "laps.h"

/**
 * print_race_state - Prints the state of the race.
 * @head: Pointer to the head node of the linked list.
 */
void print_race_state(car_node **head)
{
	car_node **current_car;

	printf("Race state:\n");
	for (current_car = head; *current_car; current_car = &(*current_car)->next)
		printf("Car %i [%lu laps]\n", (*current_car)->id, (*current_car)->lap_count);
}

/**
 * insert_sorted_car - Inserts a car node in the linked list in sorted order.
 * @head: Pointer to the head node of the linked list.
 * @new_car: Pointer to the new car node.
 */
void insert_sorted_car(car_node **head, car_node *new_car)
{
	car_node **current_car;

	current_car = head;
	while (*current_car && new_car->id > (*current_car)->id)
		current_car = &(*current_car)->next;
	new_car->next = *current_car;
	*current_car = new_car;
}

/**
 * create_car_node - Creates a new car node with the given id.
 * @id: Car identifier.
 *
 * Return: A new car node.
 */
car_node *create_car_node(int id)
{
	car_node *new_car;

	new_car = malloc(sizeof(*new_car));
	if (!new_car)
		return (NULL);
	new_car->id = id;
	new_car->lap_count = 0;
	new_car->next = NULL;
	return (new_car);
}

/**
 * check_new_cars - Checks for new cars and updates the race state.
 * @head: Pointer to the head node of the linked list.
 * @id_array: Array of car identifiers.
 * @array_size: Size of the id array.
 */
void check_new_cars(car_node **head, int *id_array, size_t array_size)
{
	car_node **current_car;
	size_t i;

	for (i = 0; i < array_size; ++i)
	{
		for (current_car = head; *current_car; current_car = &(*current_car)->next)
			if (id_array[i] == (*current_car)->id)
			{
				++(*current_car)->lap_count;
				break;
			}
		if (!*current_car)
		{
			printf("Car %i joined the race\n", id_array[i]);
			insert_sorted_car(head, create_car_node(id_array[i]));
		}
	}
}

/**
 * race_state - Tracks the number of laps made by cars in the race.
 * @id_array: Array of car identifiers.
 * @array_size: Size of the id array.
 */
void race_state(int *id_array, size_t array_size)
{
	static car_node *head;
	car_node *current_car;

	current_car = head;
	if (array_size)
	{
		check_new_cars(&head, id_array, array_size);
		print_race_state(&head);
	}
	else
	{
		while (current_car)
		{
			current_car = current_car->next;
			free(head);
			head = current_car;
		}
	}
}

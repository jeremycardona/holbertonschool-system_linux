#ifndef LAPS_H
#define LAPS_H
#define READ_SIZE BUFSIZ

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * struct file_descriptor_node - singly linked list of file descriptors
 * @fd: file descriptor
 * @read_flag: if list has been read
 * @line_head: pointer to head of line_node linked list
 * @next: pointer to next node
 */
typedef struct file_descriptor_node
{
	int fd;
	size_t read_flag;
	struct line_node *line_head;
	struct file_descriptor_node *next;
} file_descriptor_node;

/**
 * struct line_node - singly linked list of strings
 * @line_number: line number
 * @content: string
 * @length: size of string
 * @next: pointer to next node
 */
typedef struct line_node
{
	size_t line_number;
	char *content;
	size_t length;
	struct line_node *next;
} line_node;

char *_getline(const int fd);

#endif /* LAPS_H */

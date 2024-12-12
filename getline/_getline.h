#ifndef _GETLINE_H
#define _GETLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ_SIZE 1024

typedef struct line_head
{
	int fd;
	int bytes;
	char *buf;
	struct line_head *next;
} line_head;

char *_getline(const int fd);
char *read_line_chars(line_head *current_node);
void free_lines(line_head *head);
line_head *add_line(line_head **lines, const int fd, char *buf, int bytes);
char *read_line(line_head *current_node);

#endif /* _GETLINE_H */

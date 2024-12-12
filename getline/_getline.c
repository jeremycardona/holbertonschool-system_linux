#include "_getline.h"
#include <string.h>

/**
 * struct line_head - singly linked list of file descriptors and buffer
 * @fd: file descriptor associated with the buffer
 * @buf: buffer holding data read from the file
 * @bytes: number of bytes in the buffer
 * @next: pointer to the next node in the list
 */
static line_head *lines;

/**
 * _getline - Reads a line from the given file descriptor
 * @fd: The file descriptor to read from
 *
 * Return: A string containing the next line read, or NULL if EOF or error
 */
char *_getline(const int fd)
{
	line_head *current_node;
	char *line = NULL;

	if (fd == -1) /* Handle special case (fd == -1) for cleanup */
	{
		free_lines(lines);
		lines = NULL;
		return (NULL);
	}
	/* Search for the node corresponding to the given fd */
	for (current_node = lines; current_node != NULL;
	 current_node = current_node->next)
	{
		if (current_node->fd == fd)
			break;
	}
	if (current_node == NULL) /* If no node exists for the fd, create a new one */
	{
		char *buf = malloc(READ_SIZE);

		if (!buf)
			return (NULL);

		ssize_t bytes_read = read(fd, buf, READ_SIZE);

		if (bytes_read <= 0) /* Error or EOF */
		{
			free(buf);
			return (NULL);
		}

		current_node = add_line(&lines, fd, buf, bytes_read);
		if (!current_node)
		{
			free(buf);
			return (NULL);
		}
	}

	while (1) /* Extract a line from the buffer */
	{
		/* If there are no more bytes in the buffer, read more data */
		if (current_node->bytes <= 0)
		{
			ssize_t bytes_read = read(fd, current_node->buf, READ_SIZE);

			if (bytes_read <= 0) /* Error or EOF */
			{
				return (NULL); /* No line to return */
			}
			current_node->bytes = bytes_read;
		}
		line = read_line_from_buffer(current_node);
		if (line) /* Successfully read a line */
			break;
	}
	return (line);
}

/**
 * read_line_from_buffer - Reads a line from the buffer, handling newline characters
 * @current_node: Pointer to the current line_head node containing the buffer
 *
 * Return: A string representing the extracted line or NULL if no line found
 */
char *read_line_from_buffer(line_head *current_node)
{
	size_t i = 0;
	size_t line_len = 0;
	char *line = NULL;

	/* Scan buffer to find newline or end of data */
	while (i < (size_t)current_node->bytes)
	{
		if (line_len <= i) /* Grow the line buffer as needed */
		{
			line_len = i + READ_SIZE;
			char *temp = realloc(line, line_len);
			if (!temp)
			{
				free(line);
				return (NULL); /* Allocation failed */
			}
			line = temp;
		}

		line[i] = current_node->buf[i];
		if (current_node->buf[i] == '\n') /* End of line detected */
		{
			line[i] = '\0';
			/* Manually shift the buffer contents */
			int j;
			for (j = i + 1; j < current_node->bytes; j++)
			{
				current_node->buf[j - (i + 1)] = current_node->buf[j];
			}
			current_node->bytes -= (i + 1);
			return (line);
		}
		i++;
	}

	/* End of buffer reached without newline */
	if (i > 0)
	{
		line[i] = '\0';
		current_node->bytes = 0;
		return (line);
	}

	/* No data to return */
	free(line);
	return (NULL);
}

/**
 * free_lines - Frees the entire linked list and associated buffers
 * @head: Pointer to the head of the linked list
 */
void free_lines(line_head *head)
{
	while (head)
	{
		line_head *temp = head;

		head = head->next;
		free(temp->buf);
		free(temp);
	}
}

/**
 * add_line - Adds a new node to the linked list for the given file descriptor
 * @lines: Pointer to the head of the linked list
 * @fd: File descriptor to associate with the new node
 * @buf: Buffer containing data to store in the node
 * @bytes: Number of bytes in the buffer
 *
 * Return: A pointer to the newly added node, or NULL if allocation fails
 */
line_head *add_line(line_head **lines, const int fd, char *buf, int bytes)
{
	line_head *new_node = malloc(sizeof(line_head));

	if (!new_node)
	{
		free(buf);
		return (NULL);
	}

	new_node->fd = fd;
	new_node->buf = buf;
	new_node->bytes = bytes;
	new_node->next = *lines;
	*lines = new_node;

	return (new_node);
}

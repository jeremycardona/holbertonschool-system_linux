#include "_getline.h"

static size_t total_fd_count;
static file_descriptor_node *fd_head;

/**
 * link_nodes - links nodes together
 * @list_head: pointer to pointer to head node of linked list
 * @new_node: new node to link
 * @is_sorted: whether to sort the list
 */
void link_nodes(void *list_head, void *new_node, size_t is_sorted)
{
	line_node **char_cur;
	file_descriptor_node **fd_cur;

	if (!is_sorted)
	{
		char_cur = (line_node **) list_head;
		while (*char_cur)
			char_cur = &(*char_cur)->next;
		((line_node *) new_node)->next = *char_cur;
		*char_cur = new_node;
	}
	else
	{
		fd_cur = (file_descriptor_node **) list_head;
		while (*fd_cur && ((file_descriptor_node *) new_node)->fd > (*fd_cur)->fd)
			fd_cur = &(*fd_cur)->next;
		((file_descriptor_node *) new_node)->next = *fd_cur;
		*fd_cur = new_node;
	}
}

/**
 * create_node - creates new linked list node
 * @source: string
 * @end: bytes to copy
 * @increment_line: whether to increment line number
 * @create_fd_node: if creating file_descriptor_node
 * @fd: file descriptor
 *
 * Return: created linked list node
 */
void *create_node(char *source, size_t end, int increment_line, size_t create_fd_node, int fd)
{
	void *new_node;

	if (!create_fd_node)
	{
		static size_t line_count = 1;

		new_node = (line_node *) malloc(sizeof(line_node));
		if (!new_node)
			return (NULL);
		((line_node *) new_node)->line_number = line_count;
		if (increment_line)
			++line_count;
		((line_node *) new_node)->content = malloc((end + 1));
		if (!new_node)
			return (NULL);
		memcpy(((line_node *) new_node)->content, source, end);
		((line_node *) new_node)->content[end] = '\0';
		((line_node *) new_node)->length = end + 1;
		((line_node *) new_node)->next = NULL;
	}
	else
	{
		new_node = (file_descriptor_node *) malloc(sizeof(file_descriptor_node));
		if (!new_node)
			return (NULL);
		((file_descriptor_node *) new_node)->fd = fd;
		((file_descriptor_node *) new_node)->read_flag = 0;
		((file_descriptor_node *) new_node)->line_head = NULL;
		((file_descriptor_node *) new_node)->next = NULL;
	}
	return (new_node);
}

/**
 * realloc_and_parse - changes the size of the memory to new_size or parse lines
 * @ptr: pointer to memory block
 * @old_size: old size of memory block
 * @new_size: new size of memory block
 * @parse_line: whether to parse lines
 * @file_content: string of entire file
 * @line_head: pointer to pointer to head node of linked list
 *
 * Return: pointer to newly allocated memory
 */
void *realloc_and_parse(void *ptr, size_t old_size, size_t new_size,
		size_t parse_line, char *file_content, line_node **line_head)
{
	void *new_ptr;
	size_t i, start;
	line_node *new_line_node = NULL;

	if (parse_line)
	{
		for (start = i = 0; file_content[i]; ++i)
			if (file_content[i] == '\n')
			{
				new_line_node = create_node(&file_content[start], i - start, 1, 0, 0);
				link_nodes(line_head, new_line_node, 0);
				start = i + 1;
			}
		if (file_content[start])
		{
			new_line_node = create_node(&file_content[start], i - start, 0, 0, 0);
			link_nodes(line_head, new_line_node, 0);
		}
		if (!new_line_node)
			return ((char *) NULL);
	}
	else
	{
		if (new_size == 0)
		{
			free(ptr);
			return (NULL);
		}
		if (!ptr)
			return (malloc(new_size));
		if (new_size <= old_size)
			return (ptr);
		new_ptr = malloc(new_size);
		if (new_ptr)
		{
			memcpy(new_ptr, ptr, old_size);
			free(ptr);
		}
	}
	return (new_ptr);
}

/**
 * custom_strncat - concatenates two strings to at most n bytes
 * @destination: destination string
 * @source: string to copy
 * @n: amount of bytes to copy
 *
 * Return: pointer to resulting string destination
 */
char *custom_strncat(char *destination, const char *source, size_t n)
{
	size_t destination_len;
	size_t i;

	for (i = 0; destination[i]; ++i)
		;
	destination_len = i;
	for (i = 0; i < n && source[i]; ++i)
		destination[destination_len + i] = source[i];
	destination[destination_len + i] = '\0';

	return (destination);
}

/**
 * parse_file_descriptor - parses given fd and creates appropriate file_descriptor_node
 * @fd_head: pointer to pointer to fd_head node
 * @fd: file descriptor
 * @file_content: string to free if necessary
 *
 * Return: newly created or matching file_descriptor_node
 */
file_descriptor_node *parse_file_descriptor(file_descriptor_node **fd_head, const int fd, char *file_content)
{
	file_descriptor_node *fd_cur;
	line_node *cur, *tmp;

	if (fd == -1)
	{
		while (*fd_head)
		{
			cur = (*fd_head)->line_head;
			while (cur)
			{
				tmp = cur;
				cur = cur->next;
				free(tmp->content);
				free(tmp);
			}
			fd_cur = *fd_head;
			*fd_head = (*fd_head)->next;
			free(fd_cur);
		}
		free(file_content);
		return (NULL);
	}
	fd_cur = *fd_head;
	while (fd_cur)
	{
		if (fd == fd_cur->fd)
			break;
		fd_cur = fd_cur->next;
	}
	if (!fd_cur)
	{
		fd_cur = create_node('\0', 0, 0, 1, fd);
		if (!fd_cur)
			return (NULL);
		link_nodes(fd_head, fd_cur, 1);
		++total_fd_count;
	}
	return (fd_cur);
}

/**
 * read_line_from_fd - reads an entire line from a file descriptor
 * @fd: file descriptor to read from
 *
 * Return: null-terminated string excluding newline character
 * NULL on EOF or error
 */
char *_getline(const int fd)
{
	size_t line_size = READ_SIZE + 1;
	char *line, buffer[READ_SIZE] = {0}, *file_content = malloc(line_size);
	ssize_t byte;
	file_descriptor_node *fd_cur;
	line_node *tmp;

	if (!file_content)
		return (NULL);
	memset(file_content, 0, line_size);
	fd_cur = parse_file_descriptor(&fd_head, fd, file_content);
	if (!fd_cur)
		return (NULL);
	if (!fd_cur->read_flag)
		for (; (byte = read(fd, buffer, READ_SIZE)) > 0; fd_cur->read_flag = 1)
		{
			custom_strncat(file_content, buffer, READ_SIZE);
			line_size += READ_SIZE;
			file_content = realloc_and_parse(file_content, line_size - READ_SIZE, line_size, 0, NULL, NULL);
			if (!file_content)
				return (NULL);
			memset(buffer, 0, READ_SIZE);
		}
	if (fd_cur->read_flag)
		line = realloc_and_parse(NULL, 0, 0, 1, file_content, &fd_cur->line_head);
	free(file_content);
	if (!fd_cur->line_head)
	{
		if (total_fd_count == 1)
			free(fd_cur);
		return (NULL);
	}
	tmp = fd_cur->line_head;
	fd_cur->line_head = fd_cur->line_head->next;
	line = malloc(tmp->length);
	if (!line)
		return (NULL);
	memcpy(line, tmp->content, tmp->length);
	free(tmp->content);
	free(tmp);
	return (line);
}

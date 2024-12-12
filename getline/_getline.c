#include "_getline.h"
#include <string.h>

// Static linked list to manage lines for multiple file descriptors
static line_head *lines = NULL;

char *_getline(const int fd)
{
    line_head *current_node;
    char *line = NULL;

    // 1. Handle special case (fd == -1) for cleanup
    if (fd == -1)
    {
        free_lines(lines);
        lines = NULL;
        return (NULL);
    }

    // 2. Search for the node corresponding to the given fd
    for (current_node = lines; current_node != NULL; current_node = current_node->next)
    {
        if (current_node->fd == fd)
            break;
    }

    // 3. If no node exists for the fd, create a new one
    if (current_node == NULL)
    {
        char *buf = malloc(READ_SIZE);
        if (!buf)
            return (NULL);

        ssize_t bytes_read = read(fd, buf, READ_SIZE);
        if (bytes_read <= 0) // Error or EOF
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

    // 4. Extract a line from the buffer
    while (1)
    {
        // If there are no more bytes in the buffer, read more data
        if (current_node->bytes <= 0)
        {
            ssize_t bytes_read = read(fd, current_node->buf, READ_SIZE);
            if (bytes_read <= 0) // Error or EOF
            {
                return (NULL); // No line to return
            }
            current_node->bytes = bytes_read;
        }

        // Attempt to extract a line from the current buffer
        line = read_line_chars(current_node);
        if (line) // Successfully read a line
            break;
    }

    return line;
}

// Reads a line from the buffer, handling newline characters and shifting buffer contents
char *read_line_chars(line_head *current_node)
{
    size_t i = 0;
    size_t line_len = 0;
    char *line = NULL;

    // Scan buffer to find newline or end of data
    while (i < (size_t)current_node->bytes)
    {
        if (line_len <= i) // Grow the line buffer as needed
        {
            line_len += READ_SIZE;
            char *temp = realloc(line, line_len);
            if (!temp)
            {
                free(line);
                return (NULL);
            }
            line = temp;
        }

        line[i] = current_node->buf[i];
        if (current_node->buf[i] == '\n') // End of line detected
        {
            line[i] = '\0';
            memmove(current_node->buf, current_node->buf + i + 1, current_node->bytes - i - 1);
            current_node->bytes -= (i + 1);
            return line;
        }
        i++;
    }

    // End of buffer reached without newline
    if (i > 0)
    {
        line[i] = '\0';
        current_node->bytes = 0;
        return line;
    }

    // No data to return
    free(line);
    return NULL;
}

// Frees the entire linked list and associated buffers
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

// Adds a new node to the linked list for the given fd
line_head *add_line(line_head **lines, const int fd, char *buf, int bytes)
{
    line_head *new_node = malloc(sizeof(line_head));
    if (!new_node)
        return NULL;

    new_node->fd = fd;
    new_node->buf = buf;
    new_node->bytes = bytes;
    new_node->next = *lines;
    *lines = new_node;

    return new_node;
}
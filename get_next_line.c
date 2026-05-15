#include "get_next_line.h"

static char *read_to_buffer(int fd, char *stash)
{
    char    buffer[BUFFER_SIZE + 1];
    ssize_t bytes_read;

    while (1)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read == 0)
            break ;
        if (bytes_read < 0)
            return (NULL);
        buffer[bytes_read] = '\0';
        stash = ft_strjoin(stash, buffer);
        if (ft_strchr(buffer, '\n'))
            break ;
    }
    return (stash);
}

static char *extract_line(char *stash)
{
    char    *line;
    char    *newline_pos;
    if (!stash || !*stash)
        return (NULL);
    newline_pos = ft_strchr(stash, '\n');
    if (newline_pos)
        line = ft_substr(stash, 0, newline_pos - stash + 1);
    else
        line = ft_strdup(stash);
    return (line);
}

static char *update_stash(char *stash)
{
    char    *newline_pos;
    char    *new_stash;

    newline_pos = ft_strchr(stash, '\n');
    if (!newline_pos)
    {
        free(stash);
        return (NULL);
    }
    new_stash = ft_substr(stash, newline_pos - stash + 1, ft_strlen(stash));
    free(stash);
    return (new_stash);
}

char    *get_next_line(int fd)
{
    static char *stash;
    char        *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);

    stash = read_to_buffer(fd, stash);
    if (!stash || !stash[0])
    {
        free(stash);
        stash = NULL;
        return (NULL);
    }
    line = extract_line(stash);
    stash = update_stash(stash);
    return (line);
}
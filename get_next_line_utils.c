#include "get_next_line.h"

void    *ft_realloc(void *ptr, size_t size)
{
	int				i;
	unsigned char	*new_ptr;

	if (size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = (unsigned char *)malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	if (ptr == NULL)
		return (new_ptr);
	i = 0;
	while (i < size)
	{
		new_ptr[i] = ((unsigned char *)(ptr))[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}

int	alloc_chunk(char **line, int alloc_f, size_t line_len, size_t line_pos)
{
	if (!alloc_f)
		*line = (char *)malloc((line_len + 2) * sizeof (char));
	else
		*line = (char *)ft_realloc(*line, (line_pos + 2) * sizeof (char));
	if (*line == NULL)
		return (0);
	return (1);
}

void	copy_line(char *buf, char *line, size_t *i, size_t line_pos, size_t buf_pos, size_t line_len)
{
	*i = 0;
	while (*i < line_len)
	{
		line[line_pos - line_len + (*i)] = buf[buf_pos - line_len + (*i)];
		(*i)++;
	}
}

void	check_reaching_end(size_t *buf_pos, int *end_f, size_t rlen)
{
	if (*buf_pos == rlen)
	{
		(*buf_pos)--;
		*end_f = 1;
	}
}

void	zero_out(char **line,
				 int *read_f, int *alloc_f, int *end_f, int *again_f, int *exit_f,
				 size_t *buf_pos, size_t *line_pos, size_t *rlen, size_t *i)
{
	*line = NULL;
	*buf_pos = 0;
	*line_pos = 0;
	*read_f = 1;
	*alloc_f = 0;
	*end_f = 0;
	*again_f = 0;
	*exit_f = 0;
	*rlen = 0;
	*i = 0;
}

#include "get_next_line.h"

char	*get_next_line(int fd)
{
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif
	static char		buf[BUFFER_SIZE];
	static char		*line = NULL;
	static size_t	buf_pos = 0;
	static size_t	rlen = 0;

	static int		flags[5];

	static size_t	i = 0;
	
	static size_t	line_pos = 0;
	size_t			line_len;

	int				res;
	
	flags[EXIT] = FALSE; // exit_f
	flags[READ] = TRUE; // read_f
	flags[ALLOC] = FALSE; // alloc_f
	flags[END] = FALSE; // end_f
	flags[AGAIN] = FALSE; // again_f
	
	if (buf_pos >= rlen && buf_pos > 0 && rlen > 0)
		flags[0] = 1;
	while (1)
	{
		res = loop_alg(buf, &line, &rlen, &buf_pos, &line_pos, &line_len, &i, &fd, flags);
		if (res == 1)
			return (line);
		else if (res == 2)
			break ;
		else if (res == 3)
			continue ;
	}
	zero_out(&line, &read_f, &alloc_f, &end_f, &again_f, &exit_f, &buf_pos, &line_pos, &rlen, &i);
	return (NULL);
}

int		loop_alg(char *buf, char **line, size_t *rlen, size_t *buf_pos, size_t *line_pos, size_t *line_len, size_t *i, int *fd, int *flags)
{
		int	res;

		res = init(buf, line, fd, rlen, buf_pos, line_pos, line_len, i, flags);
		if (res == 1)
			return (1);
		else if (res == 2)
			return (2);
		if (!get_chunk(buf, line, flags, buf_pos, line_pos, line_len, rlen, i))
			return (2);
		check_reaching_end(&buf_pos, rlen, flags);
		if (buf[buf_pos] == '\n')
		{
			process_new_line(buf, line, &alloc_f, &read_f, &end_f, &again_f, &buf_pos, &line_pos, line_len, rlen, i);
			return (1);
		}
		else
		{
			if (process_end_chunk(line, &alloc_f, &read_f, &end_f, &line_pos, line_len, rlen, i))
				return (1);
			return (3);
		}
		return (0);
}

int		init(char *buf, char *line,
			 int *fd,
			 size_t *rlen, size_t *buf_pos, size_t *line_pos, size_t *line_len, size_t *i,
			 int *flags)
{
	if (flags[EXIT])
		return (2);
	if (flags[READ])
	{
		if (*rlen == BUFFER_SIZE && !flags[AGAIN] && buf[(*rlen) - 1] != '\n')
			flags[EXIT] = 1;
		flags[AGAIN] = 0;
		*buf_pos = 0;
		*rlen = 0;
		*rlen = read(*fd, buf, BUFFER_SIZE);
		if (*rlen <= 0)
		{
			if (flags[EXIT])
			{
				line[*line_pos - *line_len + *i] = '\0';
				return (1);
			}
			return (2);
		}
		flags[EXIT] = 0;
	}
	return (0);
}

/* It finds the first chunk of data in read buffer,
 * allocates memory for it and copies its content
 * into the `line` */
int	get_chunk(char *buf, char **line, int *flags, size_t *buf_pos,
				  size_t *line_pos, size_t *line_len, size_t rlen, size_t *i)
{
	*i = 0;
	while (buf[*buf_pos] != '\n' && *buf_pos < rlen)
	{
		(*buf_pos)++;
		(*i)++;
	}
	*line_len = i;
	*line_pos += *line_len;
	if (!flags[ALLOC])
		*line = (char *)malloc((*line_len + 2) * sizeof (char));
	else
		*line = (char *)ft_realloc(*line, (*line_pos + 2) * sizeof (char));
	if (*line == NULL)
		return (0);
	*i = 0;
	while (*i < *line_len)
	{
		line[*line_pos - *line_len + (*i)] = buf[*buf_pos - *line_len + (*i)];
		(*i)++;
	}
	return (1);
}

void	process_new_line(char *buf, char *line,
						 int *alloc_f, int *read_f, int *end_f, int *again_f,
						 size_t *buf_pos, size_t *line_pos, size_t line_len, size_t rlen, size_t i)
{
	line[*line_pos - line_len + i] = '\n';
	line[*line_pos - line_len + i + 1] = '\0';
	if (*buf_pos < rlen - 1 && !(*end_f))
		(*buf_pos)++;
	*line_pos = 0;
	*alloc_f = 0;
	*read_f = 0;
	*end_f = 0;
	if (*buf_pos == rlen - 1 && buf[*buf_pos] == '\n')
	{
		*read_f = 1;
		if (!(*again_f) && buf[*buf_pos - 1] == '\n')
		{
			*again_f = 1;
			*read_f = 0;
		}
	}
}

int	process_end_chunk(char *line, int *alloc_f, int *read_f, int *end_f, size_t *line_pos, size_t line_len, size_t rlen, size_t i)
{
	*alloc_f = 1;
	*read_f = 1;
	*end_f = 0;
	if (rlen < BUFFER_SIZE)
	{
		line[*line_pos - line_len + i] = '\0';
		*line_pos = 0;
		*alloc_f = 0;
		return (1);
	}
	return (0);
}

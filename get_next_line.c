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
	static int		exit_f = 0;

	static int		read_f = 1;
	static int		alloc_f = 0;
	static int		end_f = 0;
	static int		again_f = 0;

	static size_t	i = 0;
	
	static size_t	line_pos = 0;
	size_t			line_len;

	int				res;
	
	if (buf_pos >= rlen && buf_pos > 0 && rlen > 0)
		exit_f = 1;
	while (1)
	{
		res = init(buf, line, fd, &exit_f, &read_f, &again_f, &rlen, &buf_pos, line_pos, line_len, i);
		if (res == 1)
			return (line);
		else if (res == 2)
			break ;
		evaluate_chunk(buf, &buf_pos, &line_len, &line_pos, rlen);
		if (!alloc_chunk(&line, alloc_f, line_len, line_pos))
			break ;
		copy_line(buf, line, &i, line_pos, buf_pos, line_len);
		check_reaching_end(&buf_pos, &end_f, rlen);
		if (buf[buf_pos] == '\n')
		{
			process_new_line(buf, line, &alloc_f, &read_f, &end_f, &again_f, &buf_pos, &line_pos, line_len, rlen, i);
			return (line);
		}
		else
		{
			if (process_end_chunk(line, &alloc_f, &read_f, &end_f, &line_pos, line_len, rlen, i))
				return (line);
			continue;
		}
	}
	zero_out(&line, &read_f, &alloc_f, &end_f, &again_f, &exit_f, &buf_pos, &line_pos, &rlen, &i);
	return (NULL);
}

int		init(char *buf, char *line, int fd,
		int *exit_f, int *read_f, int *again_f,
		size_t *rlen, size_t *buf_pos, size_t line_pos, size_t line_len, size_t i)
{
	if (*exit_f)
		return (2);
	if (*read_f)
	{
		if (*rlen == BUFFER_SIZE && !(*again_f) && buf[(*rlen) - 1] != '\n')
			*exit_f = 1;
		*again_f = 0;
		*buf_pos = 0;
		*rlen = 0;
		*rlen = read(fd, buf, BUFFER_SIZE);
		if (*rlen <= 0)
		{
			if (*exit_f)
			{
				line[line_pos - line_len + i] = '\0';
				return (1);
			}
			return (2);
		}
		*exit_f = 0;
	}
	return (0);
}

void	evaluate_chunk(char *buf, size_t *buf_pos,
					   size_t *line_len, size_t *line_pos, size_t rlen)
{
	size_t	i;

	i = 0;
	while (buf[*buf_pos] != '\n' && *buf_pos < rlen)
	{
		(*buf_pos)++;
		i++;
	}
	*line_len = i;
	*line_pos += *line_len;
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

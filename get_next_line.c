/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:48:41 by dchernik          #+#    #+#             */
/*   Updated: 2025/04/20 01:39:07 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* The `v` array was defined as long long
 * to be able to fit into it both variables
 * of size_t and int types */
char	*get_next_line(int fd)
{
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif
	static char			buf[BUFFER_SIZE];
	static char			*line = NULL;

	static long long	v[6];
	static size_t		buf_pos;
	static size_t		line_pos;
	static size_t		rlen;
	size_t				line_len;
	static size_t		i;

	static int			flags[5] = {0, 1, 0, 0, 0};

	int					res;
	
	if (buf_pos >= rlen && buf_pos > 0 && rlen > 0)
		flags[EXIT] = 1;
	while (1)
	{
		res = loop_alg(buf, &line, &rlen, &buf_pos, &line_pos, &line_len, &i, &fd, flags);
		if (res == RET)
			return (line);
		else if (res == BREAK)
			break ;
		else if (res == CONT)
			continue ;
	}
	zero_out(&line, &buf_pos, &line_pos, &rlen, &i, flags);
	return (NULL);
}

int		loop_alg(char *buf,
				 char **line,
				 size_t *rlen,
				 size_t *buf_pos,
				 size_t *line_pos,
				 size_t *line_len,
				 size_t *i,
				 int *fd,
				 int *flags)
{
		int	res;

		res = init(buf, line, fd, rlen, buf_pos, line_pos, line_len, i, flags);
		if (res == RET)
			return (RET);
		else if (res == BREAK)
			return (BREAK);
		if (BREAK == get_chunk(buf, line, buf_pos, line_pos, line_len, rlen, i, flags))
			return (BREAK);
		check_reaching_end(buf_pos, rlen, flags);
		if (buf[*buf_pos] == '\n')
		{
			process_new_line(buf, line, buf_pos, line_pos, line_len, rlen, i, flags);
			return (RET);
		}
		else
		{
			if (process_end_chunk(line, line_pos, line_len, rlen, i, flags))
				return (RET);
			return (CONT);
		}
		return (NORM);
}

int		init(char *buf,
			 char **line,
			 int *fd,
			 size_t *rlen,
			 size_t *buf_pos,
			 size_t *line_pos,
			 size_t *line_len,
			 size_t *i,
			 int *flags)
{
	if (flags[EXIT])
		return (BREAK);
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
				(*line)[*line_pos - (*line_len) + (*i)] = '\0';
				return (RET);
			}
			return (BREAK);
		}
		flags[EXIT] = 0;
	}
	return (NORM);
}

/* It finds the first chunk of data in read buffer,
 * allocates memory for it and copies its content
 * into the `line` */
int	get_chunk(char *buf,
			  char **line,
			  size_t *buf_pos,
			  size_t *line_pos,
			  size_t *line_len,
			  size_t *rlen,
			  size_t *i,
			  int *flags)
{
	*i = 0;
	while (buf[*buf_pos] != '\n' && *buf_pos < *rlen)
	{
		(*buf_pos)++;
		(*i)++;
	}
	*line_len = *i;
	*line_pos += *line_len;
	if (!flags[ALLOC])
		*line = (char *)malloc((*line_len + 2) * sizeof (char));
	else
		*line = (char *)ft_realloc(*line, (*line_pos + 2) * sizeof (char));
	if (*line == NULL)
		return (BREAK);
	*i = 0;
	while ((*i) < (*line_len))
	{
		(*line)[(*line_pos) - (*line_len) + (*i)] = buf[(*buf_pos) - (*line_len) + (*i)];
		(*i)++;
	}
	return (NORM);
}

void	process_new_line(char *buf,
						 char **line,
						 size_t *buf_pos,
						 size_t *line_pos,
						 size_t *line_len,
						 size_t *rlen,
						 size_t *i,
						 int *flags)
{
	(*line)[*line_pos - (*line_len) + (*i)] = '\n';
	(*line)[*line_pos - (*line_len) + (*i) + 1] = '\0';
	if (*buf_pos < (*rlen) - 1 && !flags[END])
		(*buf_pos)++;
	*line_pos = 0;
	flags[ALLOC] = 0;
	flags[READ] = 0;
	flags[END] = 0;
	if (*buf_pos == (*rlen) - 1 && buf[*buf_pos] == '\n')
	{
		flags[READ]= 1;
		if (!flags[AGAIN] && buf[*buf_pos - 1] == '\n')
		{
			flags[AGAIN] = 1;
			flags[READ] = 0;
		}
	}
}

int	process_end_chunk(char **line,
					  size_t *line_pos,
					  size_t *line_len,
					  size_t *rlen,
					  size_t *i,
					  int *flags)
{
	flags[ALLOC] = 1;
	flags[READ] = 1;
	flags[END] = 0;
	if (*rlen < BUFFER_SIZE)
	{
		(*line)[*line_pos - (*line_len) + (*i)] = '\0';
		flags[ALLOC] = 0;
		*line_pos = 0;
		return (RET);
	}
	return (NORM);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:48:44 by dchernik          #+#    #+#             */
/*   Updated: 2025/04/20 15:44:48 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	init(char *buf, char **line, long long *v, int *flags)
{
	if (flags[EXIT])
		return (BREAK);
	if (flags[READ])
	{
		if (v[RLEN] == BUFFER_SIZE && !flags[AGAIN] && buf[v[RLEN] - 1] != '\n')
			flags[EXIT] = 1;
		flags[AGAIN] = 0;
		v[BUF_POS] = 0;
		v[RLEN] = 0;
		v[RLEN] = read(v[FD], buf, BUFFER_SIZE);
		if (v[RLEN] <= 0)
		{
			if (flags[EXIT])
			{
				(*line)[v[LINE_POS] - v[LINE_LEN] + v[I]] = '\0';
				return (RET);
			}
			return (BREAK);
		}
		flags[EXIT] = 0;
	}
	return (NORM);
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	unsigned char	*new_ptr;
	size_t			copy_size;
	size_t			i;

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = (unsigned char *)malloc(new_size);
	if (new_ptr == NULL)
		return (NULL);
	if (ptr == NULL)
		return (new_ptr);
	copy_size = new_size;
	if (old_size < new_size)
		copy_size = old_size;
	i = 0;
	while (i < copy_size)
	{
		new_ptr[i] = ((unsigned char *)(ptr))[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}

int	alloc_mem(char **line, long long *v, int *flags)
{
	if (!flags[ALLOC])
	{
		*line = (char *)malloc((v[LINE_LEN] + 2) * sizeof (char));
		v[PRIV_MEM_S] = (v[LINE_LEN] + 2) * sizeof (char);
	}
	else
	{
		*line = (char *)ft_realloc(*line,
				v[PRIV_MEM_S], (v[LINE_POS] + 2) * sizeof (char));
		v[PRIV_MEM_S] = (v[LINE_POS] + 2) * sizeof (char);
	}
	if (*line == NULL)
		return (BREAK);
	return (NORM);
}

void	check_reaching_end(long long *v, int *flags)
{
	if (v[BUF_POS] == v[RLEN])
	{
		v[BUF_POS]--;
		flags[END] = 1;
	}
}

void	clear_func_state(char **line, long long *v, int *flags)
{
	*line = NULL;
	v[BUF_POS] = 0;
	v[LINE_POS] = 0;
	v[RLEN] = 0;
	v[I] = 0;
	flags[EXIT] = 0;
	flags[READ] = 1;
	flags[ALLOC] = 0;
	flags[END] = 0;
	flags[AGAIN] = 0;
}

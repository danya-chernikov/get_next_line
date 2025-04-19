/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:48:44 by dchernik          #+#    #+#             */
/*   Updated: 2025/04/20 00:27:33 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	check_reaching_end(size_t *buf_pos, size_t *rlen, int *flags)
{
	if (*buf_pos == *rlen)
	{
		(*buf_pos)--;
		flags[END] = 1;
	}
}

void	zero_out(char **line, 
				 size_t *buf_pos,
				 size_t *line_pos,
				 size_t *rlen,
				 size_t *i,
				 int *flags)
{
	*line = NULL;
	*buf_pos = 0;
	*line_pos = 0;
	*rlen = 0;
	*i = 0;
	flags[EXIT] = 0;
	flags[READ] = 1;
	flags[ALLOC] = 0;
	flags[END] = 0;
	flags[AGAIN] = 0;
}

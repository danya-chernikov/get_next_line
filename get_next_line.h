#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* get_next_line.c */
char	*get_next_line(int fd);
//void	copy_line(char *buf, char *line, size_t line_pos, size_t line_len);

/* get_next_line_utils.c */
void    *ft_realloc(void *ptr, size_t size);

#endif

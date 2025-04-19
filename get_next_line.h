#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* get_next_line.c */
char	*get_next_line(int fd);

int		init(char *buf, char *line, int fd,
			 int *exit_f, int *read_f, int *again_f,
			 size_t *rlen, size_t *buf_pos, size_t line_pos, size_t line_len, size_t i);

void	evaluate_chunk(char *buf, size_t *buf_pos,
		size_t *line_len, size_t *line_pos, size_t rlen);

void	process_new_line(char *buf, char *line,
		int *alloc_f, int *read_f, int *end_f, int *again_f,
		size_t *buf_pos, size_t *line_pos, size_t line_len, size_t rlen, size_t i);


int		process_end_chunk(char *line,
						  int *alloc_f, int *read_f, int *end_f,
						  size_t *line_pos, size_t line_len, size_t rlen, size_t i);

/* get_next_line_utils.c */
void    *ft_realloc(void *ptr, size_t size);

int		alloc_chunk(char **line, int alloc_f, size_t line_len, size_t line_pos);

void	copy_line(char *buf, char *line, size_t *i, size_t line_pos, size_t buf_pos, size_t line_len);

void	check_reaching_end(size_t *buf_pos, int *end_f, size_t rlen);

void	zero_out(char **line,
				 int *read_f, int *alloc_f, int *end_f, int *again_f, int *exit_f,
				 size_t *buf_pos, size_t *line_pos, size_t *rlen, size_t *i);

#endif

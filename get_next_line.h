#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

#include <unistd.h>
#include <stdlib.h>

# define EXIT 0
# define READ 1
# define ALLOC 2
# define END 3
# define AGAIN 4
# define TRUE 1
# define FALSE 0

/* get_next_line.c */
char	*get_next_line(int fd);

int		init(char *buf, char *line,
			 int *fd,
			 size_t *rlen, size_t *buf_pos, size_t *line_pos, size_t *line_len, size_t *i,
			 int *flags)

int	get_chunk(char *buf, char **line, int *flags, size_t *buf_pos,
				  size_t *line_pos, size_t *line_len, size_t rlen, size_t *i);

void	process_new_line(char *buf, char *line,
		int *alloc_f, int *read_f, int *end_f, int *again_f,
		size_t *buf_pos, size_t *line_pos, size_t line_len, size_t rlen, size_t i);


int		process_end_chunk(char *line,
						  int *alloc_f, int *read_f, int *end_f,
						  size_t *line_pos, size_t line_len, size_t rlen, size_t i);

/* get_next_line_utils.c */
void    *ft_realloc(void *ptr, size_t size);

void	check_reaching_end(size_t *buf_pos, int *end_f, size_t rlen);

void	zero_out(char **line,
				 int *read_f, int *alloc_f, int *end_f, int *again_f, int *exit_f,
				 size_t *buf_pos, size_t *line_pos, size_t *rlen, size_t *i);

#endif

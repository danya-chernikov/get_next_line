/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:48:46 by dchernik          #+#    #+#             */
/*   Updated: 2025/04/20 02:48:48 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# define EXIT		0
# define READ		1
# define ALLOC		2
# define END		3
# define AGAIN		4

/* FD   - a file descriptor;
 * RLEN - bytes read from `fd` (FD);
 * I    - an auxiliary coutner variable;
 * RES  - result of a function execution. */
# define FD			0
# define BUF_POS	1
# define LINE_POS	2
# define RLEN		3
# define LINE_LEN	4
# define I			5
# define RES		6

/* NORM  - normal execution. Means to do nothing;
 * RET   - indicate that the `line` must be returned;
 * BREAK - break from the main loop must be performed;
 * CONT  - continue must be applied on the main loop. */
# define NORM		0
# define RET		1
# define BREAK		2
# define CONT		3

/* get_next_line.c */
char	*get_next_line(int fd);
int		loop_alg(char *buf, char **line, long long *v, int *flags);
int		get_chunk(char *buf, char **line, long long *v, int *flags);
void	process_new_line(char *buf, char **line, long long *v, int *flags);
int		process_end_chunk(char **line, long long *v, int *flags);

/* get_next_line_utils.c */
int		init(char *buf, char **line, long long *v, int *flags);
void	*ft_realloc(void *ptr, size_t size);
void	check_reaching_end(long long *v, int *flags);
void	clear_func_state(char **line, long long *v, int *flags);

#endif

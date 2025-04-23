/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdinput_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 02:39:52 by dchernik          #+#    #+#             */
/*   Updated: 2025/04/23 17:09:48 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main()
{
	int		fd;
	int		fd2;
	char	*line;

	if ((fd2 = open("inputs/result_stdin", O_WRONLY)) == -1)
	{
		perror("Cannot open the file for writing");
		return (1);
	}

	fd = 1;
	line = get_next_line(fd);
	if (line != NULL)
		write(fd2, line, strlen(line));
	while (line != NULL)
	{
		free(line);
		line = NULL;
		line = get_next_line(fd);
		if (line != NULL)
			write(fd2, line, strlen(line));
	}

	close(fd2);
	return (0);
}

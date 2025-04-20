/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchernik <dchernik@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 02:40:01 by dchernik          #+#    #+#             */
/*   Updated: 2025/04/20 02:40:02 by dchernik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int		fd;
	int		fd_res;
	int		fd2;
	int		fd2_res;

	int		bread;
	int		nlines;

	char	*line;

	if (argc < 3)
	{
		printf("Usage: ./main <file path> <file_path>\n");
		return (1);
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		perror("Cannot open the file for reading");
		return (2);
	}
	if ((fd2 = open(argv[2], O_RDONLY)) == -1)
	{
		perror("Cannot open the file for reading");
		close(fd);
		return (3);
	}
	if ((fd_res = open("tests/result", O_WRONLY)) == -1)
	{
		perror("Cannot open the file for writing");
		close(fd);
		close(fd2);
		return (4);
	}
	if ((fd2_res = open("tests/result2", O_WRONLY)) == -1)
	{
		perror("Cannot open the file for writing");
		close(fd);
		close(fd2);
		close(fd_res);
		return (5);
	}

	printf("\nREADING FROM THE FIRST FILE\n\n");

	bread = 0;
	nlines = 0;
	line = get_next_line(fd);
	if (line != NULL)
	{
		nlines++;
		bread += strlen(line);
		write(fd_res, line, strlen(line));
	}

	while (line != NULL)
	{
		printf("main(): get_next_line(fd) = \"%s\"\n", line);
		free(line);
		line = NULL;
		line = get_next_line(fd);
		if (line != NULL)
		{
			nlines++;
			bread += strlen(line);
			write(fd_res, line, strlen(line));
		}
	}

	printf("\n\nEND OF READING THE FIRST FILE\n\n");

	printf("\nbytes read = %d\n", bread);
	printf("lines found = %d\n", nlines);

	printf("\n\n\nREADING FROM THE SECOND FILE\n\n");

	bread = 0;
	nlines = 0;
	line = get_next_line(fd2);
	if (line != NULL)
	{
		nlines++;
		bread += strlen(line);
		write(fd2_res, line, strlen(line));
	}

	while (line != NULL)
	{
		printf("main(): get_next_line(fd) = \"%s\"\n", line);
		free(line);
		line = NULL;
		line = get_next_line(fd2);
		if (line != NULL)
		{
			nlines++;
			bread += strlen(line);
			write(fd2_res, line, strlen(line));
		}
	}

	printf("\nbytes read = %d\n", bread);
	printf("lines found = %d\n", nlines);

	printf("\n\nEND OF READING THE SECOND FILE\n\n");

	close(fd);
	close(fd_res);
	close(fd2);
	close(fd2_res);
	return (0);
}

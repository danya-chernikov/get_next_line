#include "get_next_line.h"

# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

int main(int argc, char *argv[])
{
	int		fd;
	int		fd2;
	int		bread;
	int		nlines;
	char	*line;

	if (argc < 2)
	{
		printf("Usage: ./main <file path>\n");
		return (1);
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		perror("Cannot open the file for reading");
		return (2);
	}
	if ((fd2 = open("tests/result", O_WRONLY)) == -1)
	{
		perror("Cannot open the file for writing");
		close(fd);
		return (3);
	}

	bread = 0;
	nlines = 0;
	line = get_next_line(fd);
	if (line != NULL)
	{
		nlines++;
		bread += strlen(line);
		write(fd2, line, strlen(line));
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
			write(fd2, line, strlen(line));
		}
	}

	printf("\nbytes read = %d\n", bread);
	printf("lines found = %d\n", nlines);

	close(fd);
	close(fd2);
	return (0);
}

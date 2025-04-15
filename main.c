#include "get_next_line.h"

#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int		fd;
	char	*line;

	if (argc < 2)
	{
		printf("Usage: ./main <file path>\n");
		return (1);
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		perror("Cannot open the file");
		return (2);
	}

	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("main(): get_next_line(fd) = \"%s\"\n", line);
		free(line);
		line = NULL;
		line = get_next_line(fd);
	}

	return (0);
}

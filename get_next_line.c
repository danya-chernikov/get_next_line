#include "get_next_line.h"

/* Returns next line read from
 * a file.
 *
 *     li            - line index;
 *
 *     bpos          - position in
 *					   a buffer where
 *                     the function
 *                     found '\n' on
 *                     previous call;
 *
 *     rlen          - number of bytes
 *				       actually read;
 *
 *     line          - line returned;
 *
 *     buf           - buffer for read()
 *				       function;
 *
 *     sline         - line length;
 *
 *     cont_f        - flag showing if
 *				       we should continue
 *				       adding symbols that
 *					   were read during the
 *				       current call to the
 *				       resulting line or not;
 *
 *     lines_num     - number of lines found;
 *
 *     cur_line_size - the total amount of memory
 *					   that is involved to store
 *					   the current string. */
char	*get_next_line(int fd)
{
	int				MAX_LINES_NUM = 100; // Maximum lines number in the read buffer

	static char		buf[BUFFER_SIZE + 1]; // hence the default buffer size will be 1
	static char		*line = NULL;

	static int		cont_f = 0;
	static int		alloc_f = 0;

	static int		line_pos = 0;
	static int		buf_pos = 0;

	static int		call_num = 1;
	static int		lines_num = 0; // number of lines found
	
	static int		rlen;

	size_t			i;
	size_t			line_len;
	size_t			buf_size;

	/*printf("%d call. lines_num = %d; rlen = %d\n\n", call_num, lines_num, rlen);
	printf("%s\n\n", buf);

	call_num++;*/

	if (!BUFFER_SIZE)
		buf_size = 1;
	else
		buf_size = BUFFER_SIZE;

	while (lines_num < MAX_LINES_NUM)
	{

		if (!cont_f)
		{
			rlen = 0;
			rlen = read(fd, buf, buf_size);
			if (rlen <= 0)
				return (NULL);
		}

		// We need cur_line_pos only when there is resting bytes after finding '\n'
		// Let's find out how many bytes we'll need to allocate to fit the found line
		i = 0;
		while (buf[buf_pos] != '\n' && buf_pos < rlen - 1)
		{
			buf_pos++;
			i++;
		}
		line_len = i;
		if (buf_pos == rlen - 1)
			line_len++; // because of previous loop condition buf_pos < rlen - 1
		line_pos += line_len;

		// If we are allocating for the first time
		if (!alloc_f)
			line = (char *)malloc((line_len + 2) * sizeof (char)); // yeah we're like "wasting" one byte
		else
			line = (char *)realloc(line, (line_pos + 2) * sizeof (char));
		if (line == NULL)
			return (NULL);

		// If we found a new line character
		if (buf[buf_pos] == '\n')
		{
			// Let's save and return the found line
			//copy_line(buf, line, line_pos, line_len);
			i = 0;
			while (i < line_len)
			{
				line[line_pos - line_len + i] = buf[buf_pos - line_len + i];
				i++;
			}
			line[line_pos - line_len + i] = '\n';
			line[line_pos - line_len + i + 1] = '\0';

			line_pos = 0;
			alloc_f = 0;
			cont_f = 1; // We'll have to continue processing the resting buffer's content
			buf_pos++; // It's important! Otherwise we'll start with '\n' on the next call

			lines_num++;

			return (line);
		}
		// if we found a line's chunk
		else // if (buf[buf_pos] != '\n')
		{
			// cur_line_pos = rlen
			// There are two possible options here
			if (rlen == buf_size) // we're at the juncture of two readings from the file
			{
				// We have to read from the `fd` again
				//copy_line(buf, line, line_pos, line_len);

				i = 0;
				while (i < line_len)
				{
					line[line_pos - line_len + i] = buf[(buf_pos + 1) - line_len + i];
					i++;
				}

				buf_pos = 0;
				cont_f = 0; // we need to read again
				alloc_f = 1; // we need to augment the amount of memory
				//line_pos++; // It's important! We are at the joint

				continue ;
				
			}
			else // rlen < buf_size
			{
				// Most likely we reached the end of the file
				// We are just returning the read line without '\n'
				//copy_line(buf, line, line_pos, line_len);
				i = 0;
				while (i < line_len)
				{
					line[line_pos - line_len + i] = buf[(buf_pos + 1) - line_len + i];
					i++;
				}
				line[line_pos - line_len + i] = '\0';

				line_pos = 0;
				buf_pos = 0;
				cont_f = 0;
				alloc_f = 0;

				lines_num++;

				return (line);
			}


		}

	} // while (lines_num < MAX_LINES_NUM)
	


	return (NULL);
}

/*void	copy_line(char *buf, char *line, size_t line_pos, size_t line_len)
{
	int	i;

	i = 0;
	while (i < line_len)
	{
		line[line_pos - line_len + i] = buf[buf_pos - line_len + i];
		i++;
	}
}*/

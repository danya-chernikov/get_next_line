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
	static char		buf[BUFFER_SIZE + 1];
	static char		*line = NULL;
	static int		cont_f = 0;
	static int		alloc_f = 0;
	static int		line_pos = 0;
	static int		buf_pos = 0;
	static int		read_cnt = 0;
	static int		rlen;

	size_t			i;
	size_t			line_len;
	size_t			buf_size;

	if (!BUFFER_SIZE)
		buf_size = 1;
	else
		buf_size = BUFFER_SIZE;

	while (1)
	{
		if (!cont_f)
		{
			rlen = 0;
			rlen = read(fd, buf, buf_size);
			if (rlen <= 0)
				break ;
			read_cnt++;
		}
		i = 0;
		while (buf[buf_pos] != '\n' && buf_pos < rlen - 1)
		{
			buf_pos++;
			i++;
		}
		line_len = i;
		if (buf_pos == rlen - 1)
			line_len++;
		line_pos += line_len;

		if (!alloc_f)
			line = (char *)malloc((line_len + 2) * sizeof (char));
		else
			line = (char *)ft_realloc(line, (line_pos + 2) * sizeof (char));
		if (line == NULL)
			break ;

		if (buf[buf_pos] == '\n')
		{
			if (rlen < buf_size && read_cnt > 1)
				buf_pos++;
			i = 0;
			while (i < line_len)
			{
				line[line_pos - line_len + i] = buf[buf_pos - line_len + i];
				i++;
			}
			if (rlen < buf_size && read_cnt > 1)
				line[line_pos - line_len + i] = '\0';
			else
			{
				line[line_pos - line_len + i] = '\n';
				line[line_pos - line_len + i + 1] = '\0';
			}
			line_pos = 0;
			alloc_f = 0;
			cont_f = 1;
			buf_pos++;
			if (rlen < buf_size && read_cnt > 1)
				cont_f = 0;
			return (line);
		}
		else
		{
			if (rlen == buf_size)
			{
				i = 0;
				while (i < line_len)
				{
					line[line_pos - line_len + i] = buf[(buf_pos + 1) - line_len + i];
					i++;
				}
				buf_pos = 0;
				cont_f = 0;
				alloc_f = 1;
				continue ;
				
			}
			else
			{
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

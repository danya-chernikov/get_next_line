#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char		buf[BUFFER_SIZE + 1]; // buffer for storing read content from `fd`

	static char		*line = NULL; // returned line

	static size_t	buf_pos = 0; // buffer position
	static size_t	line_pos = 0; // line position

	static int		read_f = 1; // 0 - do not read from `fd`; 1 - read from `fd`
	static int		alloc_f = 0; // 0 - use malloc() for a newly created string; 1 - use realloc()
	static int		end_f = 0;
	static int		again_f = 0;
	static int		exit_f = 0;

	static size_t	i = 0; // auxiliary counter
	static int		rlen = 0; // number of bytes read from `fd`
	
	size_t			line_len; // length of the read (sub)line
	size_t			buf_size; // the same as BUFFER_SIZE
	
	if (!BUFFER_SIZE)
		buf_size = 1; // default value
	else
		buf_size = BUFFER_SIZE;

	if (buf_pos >= rlen && buf_pos > 0 && rlen > 0)
		exit_f = 1;

	while (1)
	{
		if (exit_f)
			break ;
		if (read_f) // we must read from the file
		{
			if (rlen == buf_size && !again_f && buf[rlen - 1] != '\n')
				exit_f = 1;
			again_f = 0;
			buf_pos = 0;
			rlen = 0;
			rlen = read(fd, buf, buf_size); // read a chunk of data of `buf_size` size
			if (rlen <= 0) // reading error or end of file
			{
				if (exit_f)
				{
					line[line_pos - line_len + i] = '\0';
					return (line);
				}
				break ;
			}
			exit_f = 0;
		}

		// let's search for any LF occurence
		i = 0;
		// let's call it "init loop"
		while (buf[buf_pos] != '\n'
			&& buf_pos < rlen) // LF symbol may not be found at all
		{
			buf_pos++;
			i++;
		}
		line_len = i;
		line_pos += line_len;

		if (!alloc_f) // if the found line is the first line in this chunk
			line = (char *)malloc((line_len + 2) * sizeof (char));
		else // the found line is the just a part of the line found earlier
			line = (char *)ft_realloc(line, (line_pos + 2) * sizeof (char));
		if (line == NULL)
			break ;

		// let's just add this (sub)line to the returned `line` array
		i = 0;
		while (i < line_len)
		{
			line[line_pos - line_len + i] = buf[buf_pos - line_len + i];
			i++;
		}

		if (buf_pos == rlen)
		{
			buf_pos--; // in order not to exceed the read buffer boundary
			end_f = 1; // set the flag to indicate that we have reached the end of the read buffer
		}

		if (buf[buf_pos] == '\n') // we exited from init loop because LF was found
		{
			line[line_pos - line_len + i] = '\n';
			line[line_pos - line_len + i + 1] = '\0';

			// if (buf_pos < rlen && !end_f) ?
			if (buf_pos < rlen - 1 && !end_f) // in current buffer we still have data to proceed
				buf_pos++;

			line_pos = 0; // the next found line will be new one with the updated line_pos
			alloc_f = 0; // we'll have to allocate memory for the new line again using malloc()
			read_f = 0; // we do not have to read from `fd` again	
			end_f = 0;
			
			if (buf_pos == rlen - 1 && buf[buf_pos] == '\n') // and only if '\n' stands in the end
			{
				read_f = 1; // then yes we'll have to read again
				if (!again_f && buf[buf_pos - 1] == '\n')
				{
					again_f = 1;
					read_f = 0;
				}
			}
			return (line);
		}
		else // we exited from init loop because we reached the end of the buffer
		{
			alloc_f = 1; // we'll have to realloc
			read_f = 1; // we'll have to read again on the next iteration
			end_f = 0;

			// When we read less than buf_size and did not find any LF
			if (rlen < buf_size)
			{
				line[line_pos - line_len + i] = '\0';
				line_pos = 0;
				alloc_f = 0;
				return (line); // the last return (there is nothing more to read)
			}
			// WHAT IF rlen == buf_size AND THIS IS THE END OF FILE?!

			continue;
		}
		
	}
	line = NULL;
	buf_pos = 0;
	line_pos = 0;
	read_f = 1;
	alloc_f = 0;
	end_f = 0;
	again_f = 0;
	exit_f = 0;
	rlen = 0;
	i = 0;
	return (NULL);
}

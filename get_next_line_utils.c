#include "get_next_line.h"

void    *ft_realloc(void *ptr, size_t size)
{
	int				i;
	unsigned char	*new_ptr;

	if (size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = (unsigned char *)malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	if (ptr == NULL)
		return (new_ptr);
	i = 0;
	while (i < size)
	{
		new_ptr[i] = ((unsigned char *)(ptr))[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}

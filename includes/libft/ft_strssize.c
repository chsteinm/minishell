#include <stddef.h>

size_t	ft_strssize(char **strs)
{
	size_t	size;

	if (!strs)
		return (0);
	size = 0;
	while (strs[size])
		size++;
	return (size);
}

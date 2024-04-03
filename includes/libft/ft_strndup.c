#include "libft.h"

char	*ft_strndup(char *ptr, size_t len)
{
	size_t	i;
	char	*str;

	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = -1;
	while (++i < len)
		str[i] = ptr[i];
	return (str);
}
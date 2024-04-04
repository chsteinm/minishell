#include "libft.h"

void	ft_skip_wspaces(char **ptr)
{
	while (ft_iswhitespace(**ptr))
		(*ptr)++;
}

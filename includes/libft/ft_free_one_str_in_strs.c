#include "libft.h"

void	ft_free_one_str_in_strs(char **strs, int i_str)
{
	char	*cpy;

	ft_free_and_null(strs + i_str);
	while (strs[i_str++ + 1])
	{
		cpy = strs[i_str];
		strs[i_str] = strs[i_str - 1];
		strs[i_str - 1] = cpy;
	}
}

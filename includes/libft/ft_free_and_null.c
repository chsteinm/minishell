#include "libft.h"

void	ft_free_and_null(void *ptr_adrs)
{
	void	**tmp;

    if (ptr_adrs)
	{
		tmp = (void **)ptr_adrs;
		if (*tmp)
		{
    		free(*tmp);
        	*tmp = NULL;
		}
    }
}
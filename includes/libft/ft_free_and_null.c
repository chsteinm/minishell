#include "libft.h"

void	ft_free_and_null(void *ptr_adrs)
{
    if (ptr_adrs != NULL)
	{
    	free(*(void **)ptr_adrs);
        *(void **)ptr_adrs = NULL;
    }
}
#include "libft.h"

void	ft_free_and_null(void *ptr)
{
    if (ptr != NULL)
	{
    	free(*(void **)ptr);
        *(void **)ptr = NULL;
    }
}
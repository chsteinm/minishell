#include "libft.h"

void	ft_replace_white_space(char *line)
{
	while (*line)
	{
		if (ft_iswhitespace(*line))
			*line = ' ';
		line++;
	}
}

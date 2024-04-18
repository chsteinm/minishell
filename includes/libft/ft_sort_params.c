/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_params.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 15:00:49 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 19:08:26 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_swap(char **a, char **b)
{
	char	*cpy;

	cpy = *a;
	*a = *b;
	*b = cpy;
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	ft_sort_params(int argc, char **argv)
{
	int	i;

	while (--argc >= 1)
	{
		i = argc - 1;
		while (i > 0)
		{
			if (ft_strcmp(argv[i], argv[argc]) > 0)
				ft_swap(&argv[i], &argv[argc]);
			i--;
		}
	}
}

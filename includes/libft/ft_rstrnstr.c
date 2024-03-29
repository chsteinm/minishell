/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rstrnstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 23:50:59 by chrstein          #+#    #+#             */
/*   Updated: 2024/03/01 23:51:02 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_rstrnstr(char *haystack, char *needle, size_t len)
{
	size_t	len_needle;
	size_t	len_haystack;
	size_t	i;
	size_t	j;

	if (!*needle)
		return (haystack);
	len_needle = ft_strlen(needle) - 1;
	len_haystack = ft_strlen(haystack) - 1;
	i = len_haystack;
	while (i && len_haystack - i < len)
	{
		j = len_needle;
		while (needle[--j] == haystack[--i] && len_haystack - i < len)
			if (!j)
				return (haystack + i);
	}
	return (0);
}

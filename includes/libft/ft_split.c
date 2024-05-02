/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:14:53 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/22 19:42:26 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	n;

	n = 0;
	while (s && *s)
	{
		if (*s != c)
		{
			n++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (n);
}

static char	**free_strs(char **strs, int j)
{
	while (--j + 1)
		free(strs[j]);
	free(strs);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		w;
	int		j;
	int		len;
	char	**strs;

	w = count_words(s, c);
	strs = ft_calloc((w + 1), sizeof(char *));
	if (!strs)
		return (NULL);
	j = -1;
	while (++j < w)
	{
		while (*s == c)
			s++;
		len = 0;
		while (s[len] != c && s[len])
			len++;
		strs[j] = ft_strndup(s, len);
		if (!strs[j])
			return (free_strs(strs, j));
		s += len + 1;
	}
	return (strs);
}

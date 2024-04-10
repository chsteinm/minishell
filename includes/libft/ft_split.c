/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:14:53 by chrstein          #+#    #+#             */
/*   Updated: 2023/11/06 16:18:34 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	n;

	n = 0;
	while (*s)
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

	if (!s)
		return (NULL);
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

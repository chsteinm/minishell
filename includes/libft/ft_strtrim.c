/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:26:32 by chrstein          #+#    #+#             */
/*   Updated: 2023/11/06 16:28:32 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char c, char const *set)
{
	while (*set)
		if (*set++ == c)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		len;
	char	*str;

	while (*s1 && is_set(*s1, set))
		s1++;
	len = ft_strlen((char *)s1) - 1;
	while (len >= 0 && is_set(s1[len], set))
		len--;
	str = malloc(sizeof(char) * (len + 2));
	if (!str)
		return (NULL);
	str[++len] = 0;
	while (--len >= 0)
		str[len] = s1[len];
	return (str);
}

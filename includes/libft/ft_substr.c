/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:29:16 by chrstein          #+#    #+#             */
/*   Updated: 2023/11/06 16:29:42 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;

	while (*s && start--)
		s++;
	i = 0;
	while (s[i] && i < len)
		i++;
	str = malloc(sizeof(char) * ++i);
	if (!str)
		return (NULL);
	len = i;
	i = -1;
	while (++i < len - 1)
		str[i] = *s++;
	str[i] = 0;
	return (str);
}

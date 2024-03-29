/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:56 by chrstein          #+#    #+#             */
/*   Updated: 2023/11/06 16:22:59 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	s_len;
	size_t	d_len;

	s_len = ft_strlen(src);
	i = ft_strlen(dst);
	if (size < i || !size)
		return (s_len + size);
	d_len = i;
	while (i < size - 1 && *src)
		dst[i++] = *src++;
	dst[i] = 0;
	return (d_len + s_len);
}

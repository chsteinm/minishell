/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:10:07 by chrstein          #+#    #+#             */
/*   Updated: 2023/11/06 16:10:19 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	d = dest;
	s = src;
	if (d < s)
		return (ft_memcpy(dest, src, n));
	else if (d > s)
		while (n--)
			*(d + n) = *(s + n);
	return (dest);
}

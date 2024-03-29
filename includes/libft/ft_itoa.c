/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:48:20 by chrstein          #+#    #+#             */
/*   Updated: 2023/11/06 15:51:55 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	rec(unsigned int nb, char *ptr, int len)
{
	if (nb > 9)
		rec(nb / 10, ptr, len - 1);
	ptr[len - 1] = nb % 10 + 48;
}

char	*ft_itoa(int n)
{
	unsigned int		nb;
	char				str[12];
	char				*ptr;
	int					len;

	ft_bzero(str, 12);
	ptr = str;
	if (n < 0)
	{
		*ptr++ = '-';
		nb = -n;
	}
	else
		nb = n;
	len = (n == 0);
	while (n && ++len)
		n /= 10;
	rec(nb, ptr, len);
	return (ft_strdup(str));
}

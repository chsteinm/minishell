/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 17:19:47 by chrstein          #+#    #+#             */
/*   Updated: 2024/02/06 17:21:17 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	rec(unsigned int nb, char *ptr, unsigned int len_base,
const char *base)
{
	if (nb > len_base - 1)
		rec(nb / len_base, ptr + 1, len_base, base);
	*ptr = base[nb % len_base];
}

void	revers_str(char *str)
{
	int	len;
	int	i;

	len = ft_strlen(str);
	i = -1;
	while (++i < len / 2)
	{
		str[i] ^= str[len - 1 - i];
		str[len - 1 - i] ^= str[i];
		str[i] ^= str[len - 1 - i];
	}
}

char	*ft_itoa_base(int n, const char *base)
{
	unsigned int		nb;
	unsigned int		len_base;
	char				str[34];
	char				*ptr;

	ft_bzero(str, 34);
	ptr = str;
	if (n < 0)
	{
		*ptr++ = '-';
		nb = -n;
	}
	else
		nb = n;
	len_base = ft_strlen(base);
	rec(nb, ptr, len_base, base);
	revers_str(ptr);
	return (ft_strdup(str));
}

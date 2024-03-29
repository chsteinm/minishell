/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 15:00:05 by chrstein          #+#    #+#             */
/*   Updated: 2023/11/06 15:07:13 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	atoi_overflow(int sign)
{
	if (sign > 0)
		return (-1);
	else
		return (0);
}

long	ft_atol(const char *str)
{
	long	res;
	long	sign;
	long	last;

	res = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		str++;
		sign = -1;
	}
	while (*str >= '0' && *str <= '9')
	{
		last = res;
		res = res * 10;
		if (last != res / 10)
			return (atoi_overflow(sign));
		res += *str++ - 48;
	}
	return ((res * sign));
}

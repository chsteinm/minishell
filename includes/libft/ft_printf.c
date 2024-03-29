/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 20:56:07 by chrstein          #+#    #+#             */
/*   Updated: 2024/02/15 20:56:50 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_c(va_list args)
{
	ft_putchar_fd(va_arg(args, int), 1);
	return (1);
}

int	print_convert(va_list args, const char *str)
{
	if (*str == 'c')
		return (print_c(args));
	else if (*str == 's')
		return (print_str(args));
	else if (*str == 'd' || *str == 'u' || *str == 'i')
		return (print_n_base10(args, *str));
	else if (*str == 'p')
		return (print_p(args));
	else if (*str == 'x')
		return (ft_putnbr_base(va_arg(args, unsigned int), BASE16LOW, 16));
	else if (*str == 'X')
		return (ft_putnbr_base(va_arg(args, unsigned int), BASE16UP, 16));
	else
		return (write(1, str, 1));
}

int	print_printable(const char *str)
{
	int	i;

	i = -1;
	while (str[++i] != '%' && str[i])
		ft_putchar_fd(str[i], 1);
	return (i);
}

int	print(va_list args, const char *str)
{
	int	count_printed_char;
	int	i;
	int	check;

	count_printed_char = 0;
	while (*str)
	{
		if (*str == '%')
		{
			check = print_convert(args, str + 1);
			if (check == -1)
				return (-1);
			count_printed_char += check;
			str += 2;
		}
		i = print_printable(str);
		if (i == -1)
			return (-1);
		count_printed_char += i;
		str += i;
	}
	return (count_printed_char);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		count_printed_char;

	if (!str)
		return (-1);
	if (!(*str))
		return (0);
	count_printed_char = 0;
	va_start(args, str);
	count_printed_char = print(args, str);
	va_end(args);
	return (count_printed_char);
}

#include "libft.h"

int	print_c_fd(va_list args, int fd)
{
	ft_putchar_fd(va_arg(args, int), fd);
	return (1);
}

int	print_convert_fd(va_list args, const char *str, int fd)
{
	if (*str == 'c')
		return (print_c_fd(args, fd));
	else if (*str == 's')
		return (print_str_fd(args, fd));
	else if (*str == 'd' || *str == 'u' || *str == 'i')
		return (print_n_base10_fd(args, *str, fd));
	else if (*str == 'p')
		return (print_p_fd(args, fd));
	else if (*str == 'x')
		return (ft_putnbr_base_fd(va_arg(args, unsigned int), BASE16LOW, 16, fd));
	else if (*str == 'X')
		return (ft_putnbr_base_fd(va_arg(args, unsigned int), BASE16UP, 16, fd));
	else
		return (write(fd, str, 1));
}

int	print_printable_fd(const char *str, int fd)
{
	int	i;

	i = -1;
	while (str[++i] != '%' && str[i])
		ft_putchar_fd(str[i], fd);
	return (i);
}

int	print_fd(va_list args, const char *str, int fd)
{
	int	count_printed_char;
	int	i;
	int	check;

	count_printed_char = 0;
	while (*str)
	{
		if (*str == '%')
		{
			check = print_convert_fd(args, str + 1, fd);
			if (check == -1)
				return (-1);
			count_printed_char += check;
			str += 2;
		}
		i = print_printable_fd(str, fd);
		if (i == -1)
			return (-1);
		count_printed_char += i;
		str += i;
	}
	return (count_printed_char);
}

int	ft_dprintf(int fd, const char *str, ...)
{
	va_list	args;
	int		count_printed_char;

	if (!str)
		return (-1);
	if (!(*str))
		return (0);
	count_printed_char = 0;
	va_start(args, str);
	count_printed_char = print_fd(args, str, fd);
	va_end(args);
	return (count_printed_char);
}
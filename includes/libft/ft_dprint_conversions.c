#include "libft.h"

int	print_str_fd(va_list args, int fd)
{
	char	*str;

	str = va_arg(args, char *);
	if (!str)
	{
		ft_putstr_fd("(null)", fd);
		return (6);
	}
	ft_putstr_fd(str, fd);
	return (ft_strlen((char *)str));
}

int	print_p_fd(va_list args, int fd)
{
	int	i;

	i = 2;
	ft_putstr_fd("0x", fd);
	i += ft_putnbr_base_fd(va_arg(args, size_t), "0123456789abcdef", 16, fd);
	return (i);
}

int	print_n_base10_fd(va_list args, char c, int fd)
{
	int		i;
	char	*str;

	if (c == 'u')
		str = ft_utoa(va_arg(args, unsigned int));
	else
		str = ft_itoa(va_arg(args, int));
	if (!str)
		return (0);
	i = ft_strlen(str);
	ft_putstr_fd(str, fd);
	free(str);
	return (i);
}

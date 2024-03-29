#include "libft.h"

int	ft_putnbr_base_fd(size_t nb, char *base, size_t base_len, int fd)
{
	int	count;

	count = 1;
	if (nb >= base_len)
		count += ft_putnbr_base_fd(nb / base_len, base, base_len, fd);
	ft_putchar_fd(base[nb % base_len], fd);
	return (count);
}

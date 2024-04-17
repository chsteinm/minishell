#include "libft.h"

static bool	check_base(char *str, char **endptr, int base)
{
	if (base > 36 || base < 2)
	{
		if (endptr)
			*endptr = str;
		return (FALSE);
	}
	return (TRUE);
}

static void	wich_sign(long *sign, char **ptr)
{
	char	*str;

	str = *ptr;
	*sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			*sign = -1;
		str++;
	}
	*ptr = str;
}

static long	error(char *str, char **endptr, long sign)
{
	if (endptr)
		*endptr = str;
	if (sign == 1)
		return (LONG_MAX);
	return (LONG_MIN);
}

long	ft_strtol(char *str, char **endptr, int base)
{
	long	result;
	long	sign;

	if (check_base(str, endptr, base) == FALSE)
		return (0);
	ft_skip_wspaces(&str);
	wich_sign(&sign, &str);
	result = 0;
	while (*str)
	{
		if (ft_isdigit(*str))
			result = result * base + (*str - '0');
		else if (base > 10 && ft_isalpha(*str) && *str <= 'A' + base - 11)
			result = result * base + (*str - 'A' + 10);
		else if (base > 10 && ft_isalpha(*str) && *str <= 'a' + base - 11)
			result = result * base + (*str - 'a' + 10);
		else
			break;
		if (result < 0 && result > LONG_MIN)
			return (error(str, endptr, sign));
		str++;
	}
	if (endptr)
		*endptr = str;
	return (result * sign);
}

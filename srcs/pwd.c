#include "../includes/minishell.h"

int	ft_pwd(t_data *data, t_list *node)
{
	(void)node;
	ft_putstr_fd(data->pwd, 1);
	ft_putchar_fd('\n', 1);
	close_free_exit(data, SUCCESS);
	return (0);
}

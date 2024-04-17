#include "../includes/minishell.h"

int	ft_pwd(t_data *data, t_list *node)
{
	ft_putstr_fd(data->pwd, 1);
	ft_putchar_fd('\n', 1);
	// if (node->cmd[1])
	// {
	// 	ft_putstr_fd("pwd: too many arguments\n", 2);
	// 	return (1);
	// }
	(void)node;
	close_free_exit(data, SUCCESS);
	return (0);
}

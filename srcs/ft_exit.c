#include "../includes/minishell.h"

void	ft_exit(t_data *data, t_list *node)
{
	int status;

	if (node->cmd[1] != NULL && node->cmd[2] != NULL)
	{
		ft_dprintf(2, ERR_EXIT);
		return (close_free_exit(data, 0));
	}
	status = (int)ft_atol(node->cmd[1]);
	write(1, "exit\n", 5);
	close_free_exit(data, 0);
	exit(status);
}
#include "../includes/minishell.h"

// void	init_pipe(t_data *data)
// {
// 	t_list *node;
// 	node = data->cmds;

// }

void	make_pipes(t_data *data, t_list *node)
{
	if (node)
		node = node->next;
	while (node)
	{
		if (pipe(node->pipe) == -1)
		{
			perror("pipe");
			close_free_exit(data, EXIT_FAILURE);
		}
		else
			node->fds_pipe_to_close = TRUE;
		node = node->next;
	}
}

void	exec(t_data *data, t_list *cmd)
{
	make_pipes(data, cmd);

}
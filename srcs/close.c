#include "../includes/minishell.h"

void	close_fds(t_list *node)
{
	if (node->fd_out_to_close)
	{
		close(node->fd_out);
		node->fd_out_to_close = FALSE;
	}
	if (node->fd_in_to_close)
	{
		close(node->fd_in);
		node->fd_in_to_close = FALSE;
	}
	if (node->fds_pipe_to_close)
	{
		close(node->pipe[0]);
		close(node->pipe[1]);
		node->fds_pipe_to_close = FALSE;
	}
	if (node->fds_pipe_hd_to_close)
	{
		close(node->pipe_heredoc[0]);
		close(node->pipe_heredoc[1]);
		node->fds_pipe_hd_to_close = FALSE;
	}
		
}

void	free_cmds_list(t_list **head)
{
	t_list	*prev;
	t_list	*node;

	node = *head;
	while (node)
	{
		ft_free_strings(node->cmd);
		node->cmd = NULL;
		close_fds(node);
		ft_free_and_null(&node->file_in);
		ft_free_and_null(&node->file_out);
		ft_free_and_null(&node->lim);
		prev = node;
		node = node->next;
		ft_free_and_null(&prev);
	}
	*head = 0;
}

void	close_free_exit(t_data *data, int ret)
{
	ft_free_strings(data->splited_line);
	data->splited_line = NULL;
	free_cmds_list(&data->cmds);
	ft_lstclear(&data->cmd_param, &free);
	ft_free_and_null(&data->line);
	ft_free_and_null(&data->no_space_line);
	ft_free_and_null(&data->no_w_space_line);
	if (ret != EXIT_SUCCESS)
	{
		ft_free_strings(data->path);
		exit(ret);
	}
}

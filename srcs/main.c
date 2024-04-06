#include "../includes/minishell.h"

void	debug(t_list *node)
{
		ft_printf("cmd : ");
		ft_printstrs(node->cmd);
		ft_printf("file_in = %s\n", node->file_in);
		ft_printf("fd = %d, to close = %d\n", \
		node->fd_in, node->fd_in_to_close);
		ft_printf("file_out = %s\n", node->file_out);
		ft_printf("fd = %d, to close = %d, append = %d\n", \
		node->fd_out, node->fd_out_to_close, node->append_out);
		ft_printf("lim = %s\n\n", node->lim);
}

void	close_fds(t_list *node)
{
	if (node->fd_out_to_close)
		close(node->fd_out);
	if (node->fd_in_to_close)
		close(node->fd_in);
	if (node->pipe_heredoc)
	{
		close(node->pipe_heredoc[0]);
		close(node->pipe_heredoc[1]);
	}
}

void	free_cmds_list(t_list **head)
{
	t_list	*prev;
	t_list	*node;

	node = *head;
	while (node)
	{
		debug(node);
		ft_free_strings(node->cmd);
		node->cmd = NULL;
		close_fds(node);
		ft_free_and_null(&node->pipe_heredoc);
		ft_free_and_null(&node->file_in);
		ft_free_and_null(&node->file_out);
		ft_free_and_null(&node->lim);
		ft_free_and_null(&node->pipe);
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
		exit(ret);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	
	(void)argv[argc];
	ft_bzero((char *)&data, sizeof(t_data));
	data.pid = fork(); // pour le cas où on rentre $$ qui correspond au pid
	if (!data.pid--) //fork renvoit le pid du precessus actuel + 1 (qui correspond à celui de l'enfant)
		return (0);
	data.env = env;
	// write(1, CLEAR, 11); //met l'invite de commande tout en haut de la fenêtre
	while (1)
	{
		data.line = readline("mimishell: ");
		if (!data.line)
			break ;
		if (*data.line)
		{
			add_history(data.line);
			parse(&data);
			// ft_printf("%s\n", data.line);
			// exec(&data);
			close_free_exit(&data, 0);
		}
	}
	return (0);
}

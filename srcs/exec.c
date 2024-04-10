#include "../includes/minishell.h"

void	make_dup2(t_data *data, t_list *node)
{
	if (node->lim && dup2(*node->pipe_heredoc, STDIN_FILENO) == -1)
		return (perror("dup2"), close_free_exit(data, FAILURE));
	else if (node->fd_in_to_close)
	{
		if (dup2(node->fd_in, STDIN_FILENO) == -1)
			return (perror("dup2"), close_free_exit(data, FAILURE));
	}
	else if (node->fds_pipe_to_close)
	{
		if (dup2(node->pipe[0], STDIN_FILENO) == -1)
			return (perror("dup2"), close_free_exit(data, FAILURE));
	}
	if (node->fd_out_to_close)
	{
		if (dup2(node->fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2"), close_free_exit(data, FAILURE));
	}
	else if (node->next && dup2(node->next->pipe[1], STDOUT_FILENO) == -1)
		return (perror("dup2"), close_free_exit(data, FAILURE));

}

void	find_good_path(t_data *data, t_list *node)
{
	char	*cmd_with_path;
	size_t	i;

	if (!access(*node->cmd, X_OK))
		return ;
	i = -1;
	while (data->path && data->path[++i])
	{
		cmd_with_path = join_3_strs(data->path[++i], "/", *node->cmd);
		if (!cmd_with_path)
		{
			perror("Malloc");
			close_free_exit(data, FAILURE);
		}
		if (!access(cmd_with_path, X_OK))
		{
			free(*node->cmd);
			*node->cmd = cmd_with_path;
			return ;
		}
		free(cmd_with_path);
	}
	ft_dprintf(STDERR_FILENO, ERR_CNF, *node->cmd);
	close_free_exit(data, 127);
}

bool	to_exec(t_list *node)
{
	if (!node->cmd)
		return (FALSE);
	if (!node->lim && node->fd_in == -1)
		return (FALSE);
	if (node->fd_out == -1)
		return (FALSE);
	return (TRUE);
}

void	make_pipes(t_data *data, t_list *node)
{
	if (node)
		node = node->next;
	while (node)
	{
		if (pipe(node->pipe) == -1)
		{
			perror("pipe");
			close_free_exit(data, FAILURE);
		}
		else
			node->fds_pipe_to_close = TRUE;
		node = node->next;
	}
}

bool	exec_builtins_in_parent(t_data *data, t_list *node)
{
	(void)data;
	if (!to_exec(node))
		return (FALSE);
	// if (!ft_strncmp(*node->cmd, "cd", 3))
	// 	return (ft_cd(data, node), TRUE);
	// if (!ft_strncmp(*node->cmd, "export", 7))
	// 	return (ft_export(data, node), TRUE);
	// if (!ft_strncmp(*node->cmd, "unset", 6))
	// 	return (ft_unset(data, node), TRUE);
	// if (!ft_strncmp(*node->cmd, "exit", 5))
	// 	return (ft_exit(data, node), TRUE);
	return (FALSE);
}

bool	exec_builtins_in_child(t_data *data, t_list *node)
{
	(void)data;
	(void)node;
	// if (!ft_strncmp(*node->cmd, "echo", 5))
	// 	return (ft_echo(data, node), TRUE);
	// if (!ft_strncmp(*node->cmd, "pwd", 4))
	// 	return (ft_pwd(data, node), TRUE);
	// if (!ft_strncmp(*node->cmd, "env", 4))
	// 	return (ft_env(data, node), TRUE);
	// if (!ft_strncmp(*node->cmd, "cd", 3))
	// 	return (ft_cd(data, node), TRUE);
	// if (!ft_strncmp(*node->cmd, "export", 7))
	// 	return (ft_export(data, node), TRUE);
	// if (!ft_strncmp(*node->cmd, "unset", 6))
	// 	return (ft_unset(data, node), TRUE);
	// if (!ft_strncmp(*node->cmd, "exit", 5))
	// 	return (ft_exit(data, node), TRUE);
	return (FALSE);
}

void	exec_in_child(t_data *data, t_list *node)
{
	find_good_path(data, node);
	make_dup2(data, node);
	close_fds(node);
	if (exec_builtins_in_child(data, node) == FALSE)
	{
		execve(*node->cmd, node->cmd, data->env);
		perror("execve");
		close_free_exit(data, FAILURE);
	}
	return (close_free_exit(data, SUCCESS));
}

void	exec(t_data *data, t_list *node)
{
	make_pipes(data, node);
	if (data->nb_cmds == 1 && exec_builtins_in_parent(data, node))
	{
		return (close_free_exit(data, SUCCESS));
	}
	while (node)
	{
		if (to_exec(node))
		{
			node->pid = fork();
			if (node->pid == -1)
				return (perror("fork"), close_free_exit(data, FAILURE));
			if (!node->pid)
			{
				exec_in_child(data, node);
			}
		}
		node = node->next;
	}
}

#include "../includes/minishell.h"

bool	exec_builtins_in_parent(t_data *data, t_list *node)
{
	(void)data;
	if (!to_exec(node))
		return (FALSE);
	if (!ft_strncmp(*node->cmd, "cd", 3))
		return (ft_cd(data, node), TRUE);
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
	if (!ft_strncmp(*node->cmd, "env", 4))
		return (ft_printstrs(data->env), TRUE);
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
	make_dup2(data, node);
	close_all_fds(data->cmds);
	if (exec_builtins_in_child(data, node) == FALSE)
	{
		find_good_path(data, node);
		execve(*node->cmd, node->cmd, data->env);
		perror("execve");
		close_free_exit(data, FAILURE);
	}
	return (close_free_exit(data, SUCCESS), exit(SUCCESS));
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

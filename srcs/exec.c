/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:29:29 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 20:17:15 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	to_exec(t_list *node)
{
	if (!node)
		return (FALSE);
	if (!node->cmd)
		return (FALSE);
	if (!node->lim && node->fd_in == -1)
		return (FALSE);
	if (node->fd_out == -1)
		return (FALSE);
	if (!ft_strncmp(*node->cmd, "cd", 3))
		return (TRUE);
	if (!ft_strncmp(*node->cmd, "exit", 5))
		return (TRUE);
	if (!ft_strncmp(*node->cmd, "export", 7))
		return (TRUE);
	if (!node->fd_out && node->next && !to_exec(node->next))
		return (FALSE);
	return (TRUE);
}

bool	exec_builtins_in_parent(t_data *data, t_list *node)
{
	if (!to_exec(node))
		return (FALSE);
	if (!ft_strncmp(*node->cmd, "cd", 3))
		return (ft_cd(data, node), TRUE);
	if (!ft_strncmp(*node->cmd, "export", 7))
		return (ft_export(data, node), TRUE);
	if (!ft_strncmp(*node->cmd, "unset", 6))
		return (ft_unset(data, node), TRUE);
	if (!ft_strncmp(*node->cmd, "exit", 5))
		return (ft_exit(data, node), TRUE);
	return (FALSE);
}

bool	exec_builtins_in_child(t_data *data, t_list *node)
{
	if (!ft_strncmp(*node->cmd, "echo", 5))
		return (ft_echo(data, node), TRUE);
	if (!ft_strncmp(*node->cmd, "pwd", 4))
		return (ft_printf("%s\n", data->pwd), TRUE);
	if (!ft_strncmp(*node->cmd, "env", 4))
		return (ft_printstrs(data->env), TRUE);
	if (!ft_strncmp(*node->cmd, "cd", 3))
		return (ft_cd(data, node), TRUE);
	if (!ft_strncmp(*node->cmd, "export", 7))
		return (ft_export(data, node), TRUE);
	if (!ft_strncmp(*node->cmd, "unset", 6))
		return (ft_unset(data, node), TRUE);
	if (!ft_strncmp(*node->cmd, "exit", 5))
		return (ft_exit(data, node), TRUE);
	return (FALSE);
}

void	exec_in_child(t_data *data, t_list *node)
{
	make_dup2(data, node);
	close_all_fds(data->cmds);
	give_env_path(data);
	if (exec_builtins_in_child(data, node) == FALSE)
	{
		find_good_path(data, node);
		execve(*node->cmd, node->cmd, data->env);
		perror("execve");
		close_free_exit(data, FAILURE);
	}
	close_free_exit(data, SUCCESS);
	final_free(data);
	exit(data->last_status);
}

void	exec(t_data *data, t_list *node)
{
	make_pipes(data, node);
	data->nb_cmds = ft_lstsize(node);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:29:24 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 18:15:29 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	make_dup2(t_data *data, t_list *node)
{
	if (node->lim && dup2(*node->pipe_heredoc, STDIN_FILENO) == -1)
		return (perror("dup2"), close_free_exit(data, MUST_EXIT));
	else if (node->fd_in_to_close)
	{
		if (dup2(node->fd_in, STDIN_FILENO) == -1)
			return (perror("dup2"), close_free_exit(data, MUST_EXIT));
	}
	else if (node->fds_pipe_to_close)
	{
		if (dup2(node->pipe[0], STDIN_FILENO) == -1)
			return (perror("dup2"), close_free_exit(data, MUST_EXIT));
	}
	if (node->fd_out_to_close)
	{
		if (dup2(node->fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2"), close_free_exit(data, MUST_EXIT));
	}
	else if (node->next && dup2(node->next->pipe[1], STDOUT_FILENO) == -1)
		return (perror("dup2"), close_free_exit(data, MUST_EXIT));
}

void	error_cmd(t_data *data, t_list *node)
{
	if (opendir(*node->cmd))
	{
		ft_dprintf(STDERR_FILENO, ERR_IS_FILE, *node->cmd);
		close_free_exit(data, 126);
	}
	if (!access(*node->cmd, F_OK) && access(*node->cmd, R_OK))
	{
		ft_dprintf(STDERR_FILENO, ERR_DENIED, *node->cmd);
		close_free_exit(data, 126);
	}
	else
	{
		if (ft_strncmp(*node->cmd, "./", 2) == 0 || \
			ft_strncmp(*node->cmd, "/", 1) == 0)
			ft_dprintf(STDERR_FILENO, \
			"%s: No such file or directory\n", *node->cmd);
		else
			ft_dprintf(STDERR_FILENO, ERR_CNF, *node->cmd);
		close_free_exit(data, 127);
	}
}

void	find_good_path(t_data *data, t_list *node)
{
	char	*cmd_with_path;
	size_t	i;
	DIR		*dir;

	dir = opendir(*node->cmd);
	if (dir)
		return (closedir(dir), exec_check_file_error(data, node));
	if (!access(*node->cmd, X_OK))
		return ;
	i = -1;
	while (data->path && data->path[++i] && !ft_strchr(*node->cmd, '/'))
	{
		cmd_with_path = ft_join_3_strs(data->path[i], "/", *node->cmd);
		if (!cmd_with_path)
			return (perror("malloc"), close_free_exit(data, MUST_EXIT));
		if (!access(cmd_with_path, X_OK))
		{
			free(*node->cmd);
			*node->cmd = cmd_with_path;
			return ;
		}
		free(cmd_with_path);
	}
	return (error_cmd(data, node));
}

void	exec_check_file_error(t_data *data, t_list *node)
{
	if (ft_strncmp(*node->cmd, "./", 2) == 0 || \
		ft_strncmp(*node->cmd, "/", 1) == 0)
	{
		ft_dprintf(STDERR_FILENO, ERR_IS_FILE, *node->cmd);
		close_free_exit(data, 126);
	}
	else
	{
		ft_dprintf(STDERR_FILENO, ERR_CNF, *node->cmd);
		close_free_exit(data, 127);
	}
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

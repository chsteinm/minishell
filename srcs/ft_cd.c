/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:11:46 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/23 13:19:29 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	special_cases(t_data *data, t_list *node, char *oldpwd);
static int	change_dir(t_data *data, char *path);
static int	minus_case(t_data *data, char *oldpwd);

void	ft_cd(t_data *data, t_list *node)
{
	char	*oldpwd;
	char	*oldpwd_ptr;

	if (node->cmd[1] && node->cmd[2])
		return (data->last_status = FAILURE, ft_putstr_fd(ERR_CD_TMA, 2));
	oldpwd_ptr = ft_getenv(data->env, "PWD=");
	oldpwd = ft_strdup(oldpwd_ptr);
	if (!oldpwd)
		return (perror("Malloc"), close_free_exit(data, MUST_EXIT));
	data->to_free = oldpwd;
	if (special_cases(data, node, oldpwd) == SUCCESS)
		return (update_env_path(data, oldpwd));
	else if (change_dir(data, node->cmd[1]) == FAILURE)
	{
		data->last_status = FAILURE;
		return ;
	}
	update_env_path(data, oldpwd);
}

static int	dots_case(t_data *data, t_list *node)
{
	if (chdir(node->cmd[1]) == -1)
	{
		if (errno == EACCES)
			ft_dprintf(STDERR_FILENO, ERR_DENIED, node->cmd[1]);
		else
			ft_dprintf(2, ERR_CD, node->cmd[1]);
	}
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		return (perror("Malloc"), close_free_exit(data, MUST_EXIT), 1);
	return (SUCCESS);
}

static int	special_cases(t_data *data, t_list *node, char *oldpwd)
{
	if (!node->cmd[1] || ft_strncmp(node->cmd[1], "~", 1) == 0)
	{
		ft_export_env(data, "OLDPWD=", data->pwd);
		free(data->pwd);
		data->pwd = ft_strdup(ft_getenv(data->env, "HOME="));
		if (!data->pwd)
			return (perror("Malloc"), close_free_exit(data, MUST_EXIT), 1);
		ft_export_env(data, "PWD=", data->pwd);
		if (change_dir(data, data->pwd))
			return (data->last_status = FAILURE, 0);
		return (SUCCESS);
	}
	else if (ft_strncmp(node->cmd[1], "-", 1) == 0)
	{
		return (minus_case(data, oldpwd));
	}
	else if (!ft_strncmp(node->cmd[1], "..", 2) || \
				!ft_strncmp(node->cmd[1], ".", 1))
		return (dots_case(data, node));
	return (1);
}

static int	minus_case(t_data *data, char *oldpwd)
{
	char	*oldpwd_ptr;

	oldpwd_ptr = ft_getenv(data->env, "OLDPWD=");
	if (!oldpwd_ptr)
		return (data->last_status = FAILURE, \
				ft_putstr_fd("cd: OLDPWD not set\n", 2), 0);
	if (change_dir(data, oldpwd_ptr) == FAILURE)
		return (data->last_status = FAILURE, 0);
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		return (perror("Malloc"), close_free_exit(data, MUST_EXIT), 1);
	ft_export_env(data, "OLDPWD=", oldpwd);
	ft_export_env(data, "PWD=", data->pwd);
	ft_putstr_fd(data->pwd, 1);
	ft_putstr_fd("\n", 1);
	return (SUCCESS);
}

static int	change_dir(t_data *data, char *path)
{
	if (change_dir_cd_path(data, path) == SUCCESS)
		return (SUCCESS);
	if (chdir(path) == -1)
	{
		if (errno == EACCES)
			ft_dprintf(STDERR_FILENO, ERR_DENIED, path);
		else
			ft_dprintf(2, ERR_CD, path);
		return (FAILURE);
	}
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:11:46 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/12 09:20:45 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	special_cases(t_data *data, t_list *node);
static int	error_cases(t_list *node);

int	ft_cd(t_data *data, t_list *node)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (special_cases(data, node) || error_cases(node))
	{
		free(oldpwd);
		close_free_exit(data, FAILURE); // TODO: its not the good way to exit
		return (1);
	}

	data->pwd = getcwd(NULL, 0);
	if (!oldpwd || !data->pwd)
	{
		ft_putstr_fd("malloc failed\n", 2);
		close_free_exit(data, FAILURE);
	}
	ft_export_env(data, "OLDPWD=", oldpwd);
	ft_export_env(data, "PWD=", data->pwd);
	free(oldpwd);
	return (0);
}

static int	special_cases(t_data *data, t_list *node)
{
	if (ft_strncmp(node->cmd[1], "~", 1) == 0)
	{
		ft_export_env(data, "OLDPWD=", data->pwd);
		data->pwd = ft_getenv(data->env, "HOME=");
		ft_export_env(data, "PWD=", data->pwd);
		if (chdir(data->pwd) == -1)
		{
			perror("cd");
			return (1);
		}
	}
	else if (ft_strncmp(node->cmd[1], "-", 1) == 0)
	{
		ft_export_env(data, "OLDPWD=", data->pwd);
		data->pwd = ft_getenv(data->env, "OLDPWD=");
		ft_export_env(data, "PWD=", data->pwd);
		ft_putstr_fd(data->pwd, 1);
		if (chdir(data->env[1] + 4) == -1)
		{
			perror("cd");
			return (1);
		}
	}
	return (0);
}

static int	error_cases(t_list *node)
{

	if (chdir(node->cmd[1]) == -1 && ft_strncmp(node->cmd[1], "~", 1) != 0)
	{
		perror("cd");
		return (1);
	}
	if (node->cmd[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

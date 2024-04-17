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
static int	change_dir(t_list *node);
static int	minus_case(t_data *data);

void	ft_cd(t_data *data, t_list *node)
{
	char	*oldpwd;

	if (node->cmd[1] && node->cmd[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2));
	oldpwd = ft_strdup(ft_getenv(data->env, "PWD="));
	if (!oldpwd)
		return (perror("Malloc"), close_free_exit(data, FAILURE));
	if (special_cases(data, node))
		return (free(oldpwd));
	if (change_dir(node))
		return (free(oldpwd));
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("getcwd"), close_free_exit(data, FAILURE));
	ft_export_env(data, "OLDPWD=", oldpwd);
	ft_export_env(data, "PWD=", data->pwd);
	return (free(oldpwd));
}

static int	special_cases(t_data *data, t_list *node)
{
	if (!node->cmd[1] || ft_strncmp(node->cmd[1], "~", 1) == 0)
	{
		ft_export_env(data, "OLDPWD=", data->pwd);
		free(data->pwd);
		data->pwd = ft_strdup(ft_getenv(data->env, "HOME="));
		if (!data->pwd)
			return (perror("Malloc"), close_free_exit(data, FAILURE), -1);
		ft_export_env(data, "PWD=", data->pwd);
		if (chdir(data->pwd) == -1)
			perror("cd");
		return (1);
	}
	else if (ft_strncmp(node->cmd[1], "-", 1) == 0)
	{
		minus_case(data);
		return (1);
	}
	return (0);
}

static int	minus_case(t_data *data)
{
	char	*oldpwd;

	if (!ft_getenv(data->env, "OLDPWD="))
		return (ft_putstr_fd("cd: OLDPWD not set\n", 2), -1);
	oldpwd = ft_strdup(ft_getenv(data->env, "PWD="));
	if (!data->pwd)
		return (perror("Malloc"), close_free_exit(data, FAILURE), -1);
	free(data->pwd);
	data->pwd = ft_strdup(ft_getenv(data->env, "OLDPWD="));
	if (!data->pwd)
		return (free(oldpwd), perror("Malloc"), close_free_exit(data, FAILURE), -1);
	ft_export_env(data, "OLDPWD=", oldpwd);
	ft_export_env(data, "PWD=", data->pwd);
	ft_putstr_fd(data->pwd, 1);
	ft_putstr_fd("\n", 1);
	if (chdir(data->pwd) == -1)
		perror("cd");
	return (free(oldpwd), 0);
}

static int	change_dir(t_list *node)
{
	if (chdir(node->cmd[1]) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

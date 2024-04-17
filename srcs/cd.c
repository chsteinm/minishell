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
static void	minus_case(t_data *data);

int	ft_cd(t_data *data, t_list *node)
{
	char	*oldpwd;

	if (node->cmd[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	oldpwd = ft_strdup(data->pwd);
	if (special_cases(data, node))
		return (free(oldpwd), 0);
	if (change_dir(node))
		return (free(oldpwd), 1);
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	if (!oldpwd || !data->pwd)
	{
		ft_putstr_fd("malloc failed\n", 2);
		close_free_exit(data, FAILURE);
	}
	ft_export_env(data, "OLDPWD=", oldpwd);
	ft_export_env(data, "PWD=", data->pwd);
	return (free(oldpwd), 0);
}

static int	special_cases(t_data *data, t_list *node)
{
	if (!node->cmd[1])
		return (1);
	if (ft_strncmp(node->cmd[1], "~", 1) == 0)
	{
		ft_export_env(data, "OLDPWD=", data->pwd);
		data->pwd = ft_strdup(ft_getenv(data->env, "HOME="));
		if (!data->pwd)
		{
			ft_putstr_fd("malloc failed\n", 2);
			close_free_exit(data, FAILURE);
		}
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

static void	minus_case(t_data *data)
{
	char	*oldpwd;

	oldpwd = ft_strdup(ft_getenv(data->env, "PWD="));
	if (!oldpwd)
	{
		ft_putstr_fd("malloc failed\n", 2);
		close_free_exit(data, FAILURE);
	}
	data->pwd = ft_strdup(ft_getenv(data->env, "OLDPWD="));
	if (!data->pwd)
	{
		if (ft_getenv(data->env, "OLDPWD="))
			ft_putstr_fd("malloc failed\n", 2);
		else
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return ;
	}
	ft_export_env(data, "OLDPWD=", oldpwd);
	ft_export_env(data, "PWD=", data->pwd);
	ft_putstr_fd(data->pwd, 1);
	ft_putstr_fd("\n", 1);
	if (chdir(data->pwd) == -1)
		perror("cd");
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

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

static int	special_cases(t_data *data, t_list *node, char **oldpwd);
static int	error_cases(t_list *node);

int	ft_cd(t_data *data, t_list *node)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (error_cases(node))
	{
		free(oldpwd);
		return (1);
	}
	if (special_cases(data, node, &oldpwd))
		return (0);
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

static int	special_cases(t_data *data, t_list *node, char **oldpwd)
{
	if (ft_strncmp(node->cmd[1], "~", 1) == 0)
	{
		ft_export_env(data, "OLDPWD=", data->pwd);
		data->pwd = ft_strdup(ft_getenv(data->env, "HOME="));
		ft_export_env(data, "PWD=", data->pwd);
		if (chdir(data->pwd) == -1)
			perror("cd");
		
		return (free(*oldpwd), 1);
	}
	else if (ft_strncmp(node->cmd[1], "-", 1) == 0)
	{
		data->pwd = ft_strdup(ft_getenv(data->env, "OLDPWD="));
		ft_export_env(data, "PWD=", data->pwd);
		ft_export_env(data, "OLDPWD=", data->pwd);
		ft_putstr_fd(data->pwd, 1);
		ft_putstr_fd("\n", 1);
		if (chdir(data->pwd) == -1)
		{
			printf("here\n");
			perror("cd");
		}
		return (free(*oldpwd), 1);
	}
	return (0);
}

static int	error_cases(t_list *node)
{

	if (ft_strncmp(node->cmd[1], "~", 1) != 0 && ft_strncmp(node->cmd[1], "-", 1) != 0 \
		&& chdir(node->cmd[1]) == -1)
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

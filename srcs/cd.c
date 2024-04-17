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

int	ft_cd(t_data *data, t_list *node) //le int n'est pas justifie
{
	char	*oldpwd;

	if (!node->cmd[1])
		return (1); //bah non, "cd" c'est la meme chose que "cd ~" x)
	if (node->cmd[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	oldpwd = ft_strdup(data->pwd); //il faut prendre le pwd de data->env et non celui ci 
	if (!oldpwd)
		return (perror("Malloc"), close_free_exit(data, FAILURE), -1);
	if (special_cases(data, node))
		return (free(oldpwd), 0);
	if (change_dir(node))
		return (free(oldpwd), 1);
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("getcwd"), close_free_exit(data, FAILURE), -1);
	ft_export_env(data, "OLDPWD=", oldpwd);
	ft_export_env(data, "PWD=", data->pwd);
	return (free(oldpwd), 0);
}

static int	special_cases(t_data *data, t_list *node)
{
	if (ft_strncmp(node->cmd[1], "~", 1) == 0)
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

	oldpwd = ft_strdup(ft_getenv(data->env, "PWD="));
	if (!data->pwd)
		return (perror("Malloc"), close_free_exit(data, FAILURE), -1);
	free(data->pwd);
	data->pwd = ft_strdup(ft_getenv(data->env, "OLDPWD="));
	if (!data->pwd)
	{
		free(oldpwd);
		if (ft_getenv(data->env, "OLDPWD="))
			return (perror("Malloc"), close_free_exit(data, FAILURE), -1);
		else
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return (-1);
	}
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
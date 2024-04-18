/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:11:46 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/18 18:19:31 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	special_cases(t_data *data, t_list *node);
static int	change_dir(char *path);
static int	minus_case(t_data *data);

void	set_last_status(t_data *data, int i)
{
	data->last_status = i;
}

void	ft_cd(t_data *data, t_list *node)
{
	char	*oldpwd;

	if (node->cmd[1] && node->cmd[2])
		return (set_last_status(data, 1), \
				ft_putstr_fd("cd: too many arguments\n", 2));
	oldpwd = ft_strdup(ft_getenv(data->env, "PWD="));
	if (!oldpwd)
		return (perror("Malloc"), close_free_exit(data, MUST_EXIT));
	if (special_cases(data, node))
		return (free(oldpwd));
	if (change_dir(node->cmd[1]))
		return (set_last_status(data, 1), free(oldpwd));
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
			return (perror("Malloc"), close_free_exit(data, MUST_EXIT), -1);
		ft_export_env(data, "PWD=", data->pwd);
		if (change_dir(data->pwd))
			return (set_last_status(data, 1), -1);
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
		return (set_last_status(data, 1), \
				ft_putstr_fd("cd: OLDPWD not set\n", 2), -1);
	oldpwd = ft_strdup(ft_getenv(data->env, "PWD="));
	if (!oldpwd)
		return (perror("Malloc"), close_free_exit(data, MUST_EXIT), -1);
	free(data->pwd);
	data->pwd = ft_strdup(ft_getenv(data->env, "OLDPWD="));
	if (!data->pwd)
		return (free(oldpwd), perror("Malloc"), \
					close_free_exit(data, MUST_EXIT), -1);
	if (change_dir(data->pwd))
		return (set_last_status(data, 1), free(oldpwd), -1);
	ft_export_env(data, "OLDPWD=", oldpwd);
	ft_export_env(data, "PWD=", data->pwd);
	ft_putstr_fd(data->pwd, 1);
	ft_putstr_fd("\n", 1);
	return (free(oldpwd), 0);
}

static int	change_dir(char *path)
{
	if (chdir(path) == -1)
	{
		if (errno == EACCES)
			ft_dprintf(STDERR_FILENO, ERR_DENIED, path);
		else
			ft_dprintf(2, ERR_CD, path);
		return (1);
	}
	return (0);
}

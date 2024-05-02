/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:26:16 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/23 13:18:18 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	malloc_error(t_data *data, char *value)
{
	free(value);
	perror("Malloc");
	close_free_exit(data, MUST_EXIT);
}

void	update_env_path(t_data *data, char *old)
{
	size_t	j_env;

	if (!*old)
	{
		j_env = 0;
		while (data->env[j_env] && !ft_strnstr(data->env[j_env], "OLDPWD=", 7))
			j_env++;
		return (ft_free_one_str_in_strs(data->env, j_env));
	}
	if (!ft_getenv(data->env, "OLDPWD="))
	{
		data->tmp = ft_strjoin("OLDPWD=", old);
		if (!data->tmp)
			malloc_error(data, old);
		add_var_to_env(data, &data->tmp);
		ft_free_and_null(&data->tmp);
	}
	ft_export_env(data, "OLDPWD=", old);
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
		return (perror("getcwd"), close_free_exit(data, FAILURE));
	ft_export_env(data, "PWD=", data->pwd);
}

void	ft_export_env(t_data *data, char *name, char *value)
{
	int		i;
	char	*tmp;

	i = 0;
	while (data->env[i])
	{
		if (!(ft_strncmp(data->env[i], name, ft_strlen(name))))
		{
			tmp = ft_strdup(name);
			if (!tmp)
				return (malloc_error(data, value));
			free(data->env[i]);
			if (ft_strncmp(tmp, value, ft_strlen(value)))
				data->env[i] = ft_strjoin(tmp, value);
			else
				data->env[i] = ft_strdup(tmp);
			if (!data->env[i])
				return (malloc_error(data, value));
			free(tmp);
		}
		i++;
	}
}

char	*ft_getenv(char **env, char *name)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, len))
			return (env[i] + len);
		i++;
	}
	return (NULL);
}

int	change_dir_cd_path(t_data *data, char *path)
{
	size_t	i;
	char	**cd_path;
	char	*added_path;

	cd_path = ft_split(ft_getenv(data->env, "CDPATH="), ':');
	if (!cd_path)
		return (malloc_error(data, path), -1);
	i = -1;
	while (cd_path[++i])
	{
		added_path = ft_join_3_strs(cd_path[i], "/", path);
		if (!added_path)
		{
			ft_free_strings(cd_path);
			return (malloc_error(data, path), -1);
		}
		if (!chdir(added_path))
		{
			ft_free_strings(cd_path);
			return (free(added_path), SUCCESS);
		}
		free(added_path);
	}
	ft_free_strings(cd_path);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:11:46 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/11 19:30:21 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_export_env(t_data *data, char *name, char *value)
{
	int		i;
	char	*tmp;

	i = 0;
	while (data->env[i])
	{
		if (!ft_strncmp(data->env[i], name, ft_strlen(name)))
		{
			tmp = ft_strdup(name);
			// free(data->env[i]);
			data->env[i] = ft_strjoin(tmp, value);
			printf("DEBUG env[i]: %s\n", data->env[i]);
			// free(tmp);
			// return (data->env);
		}
		i++;
	}
	// return (data->env);
}

void	ft_cd(t_data *data, t_list *node)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = getcwd(NULL, 0);

	if (chdir(node->cmd[1]) == -1)
	{
		error(data, FAILURE, node->cmd[1][0]);
		free(oldpwd);
		return ;
	}
	pwd = getcwd(NULL, 0);
	data->pwd = pwd;

	printf("pwd: %s\n", node->cmd[1]);
	printf("pwd: %s\n", pwd);
	printf("oldpwd: %s\n", oldpwd);

	ft_export_env(data, "OLDPWD=", oldpwd);
	ft_export_env(data, "PWD=", pwd);
	// free(oldpwd);
	// free(pwd);


}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:11:46 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/11 19:17:09 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_export_env(char **env, char *name, char *value)
{
	int		i;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, ft_strlen(name)))
		{
			tmp = ft_strdup(name);
			// free(env[i]);
			env[i] = ft_strjoin(tmp, value);
			printf("DEBUG env[i]: %s\n", env[i]);
			// free(tmp);
			return (env);
		}
		i++;
	}
	return (env);
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

	data->env = ft_export_env(data->env, "OLDPWD=", oldpwd);
	data->env = ft_export_env(data->env, "PWD=", pwd);
	// free(oldpwd);
	// free(pwd);


}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 20:29:09 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/18 20:30:30 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	give_env_path(t_data *data)
{
	char	*ptr;

	ptr = NULL;
	ptr = ft_getenv(data->env, "PATH=");
	if (!ptr)
		return ;
	ft_free_strings(data->path);
	data->path = ft_split(ptr, ':');
	if (!data->path)
	{
		perror("malloc");
		close_free_exit(data, FAILURE);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:26:06 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/18 16:26:07 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_echo(t_data *data, t_list *node)
{
	int	i;
	int	newline;

	i = 1;
	newline = 0;
	if (node->cmd[1] && !ft_strncmp(node->cmd[1], "-n", 3))
	{
		i++;
		newline = 1;
	}
	while (node->cmd[i])
	{
		ft_putstr_fd(node->cmd[i], 1);
		if (node->cmd[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!newline)
		ft_putstr_fd("\n", 1);
	data->last_status = 0;
}

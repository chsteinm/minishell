/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:30:09 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/22 18:04:12 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	arg_error(t_data *data, t_list *node, int error)
{
	if (error == 2)
	{
		ft_dprintf(2, ERR_EXIT_N, node->cmd[1]);
		data->last_status = error;
	}
	else
	{
		ft_dprintf(2, ERR_EXIT);
		data->last_status = error;
	}
	return (close_free_exit(data, 0));
}

void	ft_exit(t_data *data, t_list *node)
{
	char	*endptr;

	if (node->cmd[1] != NULL && node->cmd[2] != NULL)
		return (arg_error(data, node, 1));
	if (node->cmd[1] != NULL)
	{
		endptr = node->cmd[1];
		data->last_status = (int)ft_strtol(node->cmd[1], &endptr, 10);
		if (*endptr)
			return (arg_error(data, node, 2));
	}
	if (data->last_status > 255)
		data->last_status = data->last_status % 256;
	write(1, "exit\n", 5);
	close_free_exit(data, 0);
	exit(data->last_status);
}

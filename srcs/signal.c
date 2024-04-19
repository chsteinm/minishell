/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:57:31 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/18 19:10:21 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_dprintf(1, "  \b\b");
	}
}

void	handle_signal(t_data *data)
{
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		perror("signal");
		close_free_exit(data, FAILURE);
	}
	if (signal(SIGQUIT, sig_handler) == SIG_ERR)
	{
		perror("signal");
		close_free_exit(data, FAILURE);
	}
}

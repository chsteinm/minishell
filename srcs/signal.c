/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:57:31 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/20 22:43:07 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = CTRL_C;

		ft_putstr_fd("\n", 0);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		if (g_signal == IN_LOOP)
			ft_putstr_fd("Quit: 3\n", 1);
		else
		{
			rl_on_new_line();
			rl_redisplay();
			ft_dprintf(1, "  \b\b");
		}
	}
}

static void	heredoc_sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = CTRL_C;
		ft_putstr_fd("\n", 0);
	}
}

void	signal_set_status(t_data *data)
{
	if (g_signal == CTRL_C)
	{
		g_signal = STANDBY;
		data->last_status = 130;
	}
}

void	handle_signal(t_data *data)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		close_free_exit(data, FAILURE);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		close_free_exit(data, FAILURE);
	}
}

void	heredoc_handle_signal(t_data *data)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = heredoc_sig_handler;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		close_free_exit(data, FAILURE);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		close_free_exit(data, FAILURE);
	}
	signal_set_status(data);
}

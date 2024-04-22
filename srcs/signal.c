/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:57:31 by guilrodr          #+#    #+#             */
/*   Updated: 2024/04/22 18:13:23 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = CTRL_C;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
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
	sa.sa_flags = SA_RESTART;
	signal(SIGINT, SIG_DFL);
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

static void	heredoc_sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = CTRL_C;
		ft_putstr_fd("\n", 1);
		write(1, "\0", 0);
		close(0);
	}
	else if (signum == SIGQUIT)
	{
		g_signal = 2;
		ft_dprintf(1, "\b\b  \b\b");
	}
}

void	heredoc_handle_signal(t_data *data)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = heredoc_sig_handler;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		close_free_exit(data, FAILURE);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		close_free_exit(data, FAILURE);
	}
	signal_set_status(data);
}

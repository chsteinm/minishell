/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:30:33 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/23 13:46:08 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal;

void	wait_all_pid(t_data *data)
{
	t_list	*node;
	bool	must_exit;

	must_exit = FALSE;
	node = data->cmds;
	if (g_signal == CTRL_C)
		data->last_status = 130;
	close_all_fds(node);
	while (node)
	{
		if (node->pid)
		{
			waitpid(node->pid, &data->last_status, 0);
			if (WIFEXITED(data->last_status))
				data->last_status = WEXITSTATUS(data->last_status);
			else if (WIFSIGNALED(data->last_status))
				data->last_status = WTERMSIG(data->last_status) + 128;
		}
		node = node->next;
		if (data->last_status == MUST_EXIT)
			must_exit = TRUE;
		signal_set_status(data);
	}
	if (must_exit == TRUE)
		close_free_exit(data, FAILURE);
}

void	init_data(t_data *data, char **env)
{
	ft_bzero((char *)data, sizeof(t_data));
	data->pid = fork();
	if (data->pid == -1)
	{
		perror("fork");
		close_free_exit(data, FAILURE);
	}
	if (!data->pid--)
		exit(0);
	data->env = ft_strsdup(env, ft_strssize(env));
	if (!data->env)
	{
		perror("malloc");
		close_free_exit(data, FAILURE);
	}
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
	{
		perror("getcwd");
		close_free_exit(data, FAILURE);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argv[argc];
	init_data(&data, env);
	write(1, CLEAR, 10);
	while (1)
	{
		handle_signal(&data);
		g_signal = STANDBY;
		data.line = readline("mimishell: ");
		if (!data.line)
			break ;
		signal_set_status(&data);
		if (*data.line)
		{
			add_history(data.line);
			parse(&data);
			exec(&data, data.cmds);
			wait_all_pid(&data);
			close_free_exit(&data, 0);
		}
	}
	final_free(&data);
	return (0);
}

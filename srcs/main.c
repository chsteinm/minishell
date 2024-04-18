/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:30:33 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 15:48:44 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	wait_all_pid(t_data *data)
{
	t_list	*node;

	node = data->cmds;
	close_all_fds(node);
	while (node)
	{
		if (node->pid)
			waitpid(node->pid, &data->last_status, 0);
		node = node->next;
	}
	if (WEXITSTATUS(data->last_status))
		data->last_status = WEXITSTATUS(data->last_status);
}

void	give_env_path(t_data *data)
{
	char	*ptr;

	ptr = NULL;
	ptr = getenv("PATH");
	if (!ptr)
		return ;
	data->path = ft_split(ptr, ':');
	if (!data->path)
	{
		perror("malloc");
		close_free_exit(data, FAILURE);
	}
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
	give_env_path(data);
	data->pwd = getcwd(NULL, 0);
	if (!data->pwd)
	{
		perror("getcwd");
		close_free_exit(data, FAILURE);
	}
}

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argv[argc];
	init_data(&data, env);
	signal(SIGINT, sig_handler);
	signal(SIGOUT, sig_handler);
	// write(1, CLEAR, 10);
	while (1)
	{
		data.line = readline("minishell: ");
		if (!data.line)
			break ;
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

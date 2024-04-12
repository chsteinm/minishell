#include "../includes/minishell.h"

void	debug(t_list *node)
{
	sleep(1);
	while (node)
	{
		dprintf(2, "\ncmd : ");
		if (node->cmd)
		{
			for (int i = 0; node->cmd[i]; i++)
				dprintf(2, "[%s] ", node->cmd[i]);
		}
		else
			dprintf(2, "(null)");
		dprintf(2, "\n");
		dprintf(2, "file_in = %s\n", node->file_in);
		dprintf(2, "fd = %d, to close = %d\n", \
		node->fd_in, node->fd_in_to_close);
		dprintf(2, "file_out = %s\n", node->file_out);
		dprintf(2, "fd = %d, ", node->fd_out);
		dprintf(2, "to close = %d, ", node->fd_out_to_close);
		dprintf(2, "append = %d\n", node->append_out);
		dprintf(2, "lim = %s, ", node->lim);
		dprintf(2, "pipe_hd = %d\n", node->fds_pipe_hd_to_close);
		dprintf(2, "pipe = %d\n\n", node->fds_pipe_to_close);
		node = node->next;
	}
}

void	wait_all_pid(t_data *data)
{
	t_list	*node;

	node = data->cmds;
	// debug(node);
	close_all_fds(node);
	while (node)
	{
		if (node->pid)
			waitpid(node->pid, &data->last_status, 0);
		node = node->next;
	}
	if (WEXITSTATUS(data->last_status))
		data->last_status = WEXITSTATUS(data->last_status);
	// dprintf(2, "last status = %d\n", data->last_status);
}

void	give_env_path(t_data *data)
{
	char	*ptr;
	size_t	i;

	i = -1;
	ptr = NULL;
	while(!ptr && data->env[++i])
		ptr = ft_strnstr(data->env[i], "PATH=", 5);
	if (!ptr)
		return;
	data->path = ft_split(ptr, ':');
	if (!data->path)
	{
		perror("Malloc");
		close_free_exit(data, FAILURE);
	}
}

void	init_data(t_data *data, char **env)
{
	ft_bzero((char *)data, sizeof(t_data));
	data->pid = fork(); // pour le cas où on rentre $$ qui correspond au pid
	if (data->pid == -1)
	{
		perror("fork");
		close_free_exit(data, FAILURE);
	}
	if (!data->pid--) //fork renvoit le pid du precessus actuel + 1 (qui correspond à celui de l'enfant)
		exit(0);
	data->env = env;
	give_env_path(data);
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
	signal(SIGINT, sig_handler);
	signal(SIGOUT, sig_handler);
	init_data(&data, env);
	// write(1, CLEAR, 11); //met l'invite de commande tout en haut de la fenêtre
	while (1)
	{
		data.line = readline("mimishell: ");
		if (!data.line)
			break ;
		if (*data.line)
		{
			add_history(data.line);
			parse(&data);
			// dprintf(2, "%s\n", data.line);
			exec(&data, data.cmds);
			wait_all_pid(&data);
			close_free_exit(&data, 0);
		}
	}
	ft_free_strings(data.path);
	return (0);
}

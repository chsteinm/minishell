#include "../includes/pipex_bonus.h"

void	open_files(t_data *data, int argc, char **argv)
{
	data->fd_in = open(argv[1], O_RDONLY);
	if (data->fd_in == -1)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s\n", argv[1], \
		strerror(errno));
	}
	if (data->is_here_doc)
		data->fd_out = open(argv[argc - 1], \
		O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		data->fd_out = open(argv[argc - 1], \
		O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (data->fd_out == -1)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s\n", argv[argc - 1], \
		strerror(errno));
	}
}

char	**ret_path(int *is_env, int *is_path, char **env)
{
	char	*line_ptr;
	size_t	i;

	if (env && *env)
		*is_env = 1;
	else
		return (NULL);
	i = -1;
	line_ptr = NULL;
	while (!line_ptr && env[++i])
		line_ptr = ft_strnstr(env[i], "PATH=", 6);
	if (!line_ptr)
		return (NULL);
	else
		*is_path = 1;
	return (ft_split(line_ptr, ':'));
}

void	set_fildes(t_data *data, int ac)
{
	int	i;

	data->nb_cmd = ac - 1 - data->is_here_doc - 2;
	data->fildes = ft_calloc(data->nb_cmd, sizeof(int *));
	if (!data->fildes)
	{
		perror("malloc");
		close_free_exit(data, EXIT_FAILURE);
	}
	i = -1;
	while (++i < data->nb_cmd - 1)
	{
		data->fildes[i] = ft_calloc(2, sizeof(int));
		if (!data->fildes[i])
		{
			perror("malloc");
			close_free_exit(data, EXIT_FAILURE);
		}
		if (pipe(data->fildes[i]) == -1)
		{
			perror("pipe");
			close_free_exit(data, EXIT_FAILURE);
		}
	}
}

void	set_fork(t_data *data)
{
	int	i;

	data->pid = malloc(data->nb_cmd * sizeof(int));
	if (!data->pid)
	{
		perror("malloc");
		close_free_exit(data, EXIT_FAILURE);
	}
	i = -1;
	while (++i < data->nb_cmd)
	{
		data->pid[i] = fork();
		if (data->pid[i] == -1)
		{
			perror("fork");
			close_free_exit(data, EXIT_FAILURE);
		}
		if (!data->pid[i])
			break ;
	}
}

void	set(t_data *data, int argc, char **argv, char **env)
{
	open_files(data, argc, argv);
	data->argv_ptr = argv + 1 + data->is_here_doc;
	data->env_ptr = env;
	if (data->is_here_doc)
		here_doc_manage(data);
	data->path = ret_path(&data->is_env, &data->is_path, env);
	if (!data->path && data->is_env && data->is_path)
	{
		perror("malloc");
		close_free_exit(data, EXIT_FAILURE);
	}
	set_fildes(data, argc);
	set_fork(data);
}
#include "../includes/pipex_bonus.h"

char	*join_3(char *s1, char *s2, char *s3)
{
	char	*ret;
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	ret = ft_strjoin(tmp, s3);
	free(tmp);
	return (ret);
}

char	**ret_args(char **args, char *argv_str, char **path)
{
	char	*good_path;
	char	*tmp;

	args = ft_split(argv_str, ' ');
	if (!args || !*args)
		return (free(args), NULL);
	good_path = args[0];
	tmp = NULL;
	while (path && access(good_path, X_OK) && *path && \
	!ft_strnstr(*args, "/", INT_MAX))
	{
		free(tmp);
		good_path = join_3(*path++, "/", args[0]);
		if (!good_path)
			return (NULL);
		tmp = good_path;
	}
	if (path && *path && tmp)
	{
		free(args[0]);
		args[0] = good_path;
	}
	else if (tmp)
		free(good_path);
	return (args);
}

void	exec_cmd(t_data *data)
{
	data->args = ret_args(data->args, data->argv_ptr[data->i], data->path);
	if (!data->args)
	{
		perror("malloc");
		close_free_exit(data, EXIT_FAILURE);
	}
	if (access(*data->args, X_OK))
	{
		ft_dprintf(STDERR_FILENO, ERR_CNF, *data->args);
		close_free_exit(data, 127);
	}
	if (data->i == 1 && dup2(data->fd_in, STDIN_FILENO) == -1)
		return (perror("dup2"), close_free_exit(data, EXIT_FAILURE));
	if (data->i > 1 && dup2(data->fildes[data->i - 2][0], STDIN_FILENO) == -1)
		return (perror("dup2"), close_free_exit(data, EXIT_FAILURE));
	if (data->i == data->nb_cmd && dup2(data->fd_out, STDOUT_FILENO) == -1)
		return (perror("dup2"), close_free_exit(data, EXIT_FAILURE));
	if (data->i < data->nb_cmd)
		if (dup2(data->fildes[data->i - 1][1], STDOUT_FILENO) == -1)
			return (perror("dup2"), close_free_exit(data, EXIT_FAILURE));
	close_fds(data);
	execve(*data->args, data->args, data->env_ptr);
	perror(data->args[0]);
	close_free_exit(data, 0);
	exit(EXIT_FAILURE);
}

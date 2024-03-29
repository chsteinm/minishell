#include "../includes/pipex.h"

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

int	exec_second(t_data *data, char **env)
{
	data->args_out = ret_args(data->args_out, data->argv_ptr[3], data->path);
	if (!data->args_out)
		return (perror("malloc"), close_n_exit(data, 1));
	if (access(*data->args_out, X_OK))
		return (perror(*data->args_out), close_n_exit(data, 127));
	if (data->fd_out != -1)
	{
		if (data->fd_in != -1)
			close(data->fd_in);
		if (dup2(data->fildes[0], STDIN_FILENO) == -1)
			return (perror("dup2"), close_n_exit(data, 1));
		if (dup2(data->fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2"), close_n_exit(data, 1));
		close(data->fildes[1]);
		close(data->fildes[0]);
		close(data->fd_out);
		execve(data->args_out[0], data->args_out, env);
		perror(data->args_out[0]);
		return (close_n_exit(data, 1));
	}
	else
		return (close_n_exit(data, 1));
}

int	exec_first(t_data *data, char **env)
{
	data->args_in = ret_args(data->args_in, data->argv_ptr[2], data->path);
	if (!data->args_in)
		return (close_n_exit(data, 1));
	if (data->fd_in != -1)
	{
		if (data->fd_out != -1)
			close(data->fd_out);
		if (dup2(data->fd_in, STDIN_FILENO) == -1)
			return (perror("dup2"), close_n_exit(data, 1));
		if (dup2(data->fildes[1], STDOUT_FILENO) == -1)
			return (perror("dup2"), close_n_exit(data, 1));
		close(data->fd_in);
		close(data->fildes[0]);
		close(data->fildes[1]);
		execve(data->args_in[0], data->args_in, env);
		perror(data->args_in[0]);
		return (close_n_exit(data, 1));
	}
	else
		return (close_n_exit(data, 0));
}

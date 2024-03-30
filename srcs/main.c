#include "../includes/minishell.h"

// void	close_fds(t_data *data)
// {
// 	int	i;

// 	if (data->fd_in != -1 && !data->is_here_doc)
// 		close(data->fd_in);
// 	if (data->fd_out != -1)
// 		close(data->fd_out);
// 	if (data->is_here_doc)
// 	{
// 		close(data->fildes_hd[0]);
// 		close(data->fildes_hd[1]);
// 	}
// 	i = -1;
// 	while (data->fildes && data->fildes[++i])
// 	{
// 		close(data->fildes[i][0]);
// 		close(data->fildes[i][1]);
// 		free(data->fildes[i]);
// 	}
// }

void	close_free_exit(t_data *data, int ret)
{
	// if (ret)
	// 	close_fds(data);
	ft_free_strings(data->splited_line);
	free(data->line);
	free(data->no_space_line);
	if (ret != EXIT_SUCCESS)
		exit(ret);
}

int		is_in_quote(char *line, char *ptr, char q)
{
	int s_quote_left;
	int s_quote_right;

	s_quote_left = 0;
	s_quote_right = 0;
	while (line != ptr)
		if (*line++ == q)
			s_quote_left++;
	while (*ptr)
		if (*ptr++ == q)
			s_quote_right++;
	return (s_quote_left % 2 && s_quote_right % 2);
}

char	*join_3_strs(char *s1, char *s2, char *s3)
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

void	replace_var(t_data *data, char *ptr)
{
	char	*to_rep;
	char	*var;
	size_t	len;
	size_t	i;

	len = 0;
	while (ptr[++len] && !ft_iswhitespace(ptr[len]) && \
	!ft_ismeta(ptr[len]) && ptr[len] != '$')
		continue;
	to_rep = ft_calloc(len + 1, 1);
	if (!to_rep)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
	ft_strlcpy(to_rep, ptr + 1, len);
	to_rep[len - 1] = '=';
	var = NULL;
	i = -1;
	while (!var && data->env[++i])
		var = ft_strnstr(data->env[i], to_rep, len);
	if (var)
		var += len;
	free(to_rep);
	*ptr = 0;
	ptr[len - 1] = 0;
	data->tmp = data->line;
	data->line = join_3_strs(data->line, var, ptr + len);
	free(data->tmp);
	if (!data->line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
}

void	replace_pid(t_data *data, char *ptr)
{
	char	*pid;
	char	*tmp;

	pid = ft_itoa(data->pid);
	if (!pid)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
	*ptr = 0;
	ptr[1] = 0;
	tmp = data->line;
	data->line = join_3_strs(data->line, pid, ptr + 2);
	free(tmp);
	free(pid);
	if (!data->line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
}

void	expend(t_data *data)
{
	char	*ptr;
	size_t	i;

	ptr = data->line;
	i = -1;
	while (ptr[++i])
	{
		if (ptr[i] == '$' && ptr[i + 1] && !is_in_quote(ptr, ptr + i, '\''))
		{
			if (ptr[i + 1] == '$')
			{
				replace_pid(data, ptr + i);
				return (expend(data));
			}
			else if (ft_iswhitespace(ptr[i + 1]) || ft_ismeta(ptr[i + 1]))
				continue;
			else if (ptr[i + 1] != '"' && ptr[i + 1] != '\'')
			{
				replace_var(data, ptr + i);
				return (expend(data));
			}
		}
	}
}

void	error(t_data *data, int error, char c)
{
	if (error == 2 && c)
		ft_dprintf(2, ERR_SYNTX, c);
	else if (error == 2 && !c)
		ft_dprintf(2, ERR_SYNTX_NL);
	else if (error == 'q')
		ft_dprintf(2, ERR_QUOTE);
	if (error != 2)
		data->last_signal = 1;
	else
		data->last_signal = error;
}

int	check_syntax(t_data *data)
{
	char *ptr;

	data->no_space_line = ft_strdelspace(data->line);
	if (!data->no_space_line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE), 1);
	ptr = data->no_space_line;
	while (*ptr)
	{
		if (ft_ismeta(*ptr) && \
		!is_in_quote(data->no_space_line, ptr, '\'') && \
		!is_in_quote(data->no_space_line, ptr, '"'))
		{
			if (*ptr == '|' && (ptr[1] == '|' || \
			ptr[1] == '\0' || ptr == data->no_space_line))
				return (error(data, 2, *ptr), 1);
			else if (*ptr != ptr[1] && (ft_ismeta(ptr[1]) || ptr[1] == '\0'))
				return (error(data, 2, ptr[1]), 1);
			else if (*ptr == ptr[1] && (ft_ismeta(ptr[2]) || ptr[1] == '\0'))
				return (error(data, 2, ptr[2]), 1);
		}
		ptr++;
	}
	return (EXIT_SUCCESS);
}

bool	check_quote(t_data *data)
{
	bool	in_quote;
	bool	in_s_quote;
	char	*ptr;

	in_quote = FALSE;
	in_s_quote = FALSE;
	ptr = data->line;
	while (*ptr)
	{
		if (*ptr == '"' && !in_s_quote)
		{
			if (!in_quote)
				in_quote = TRUE;
			else
				in_quote = FALSE;
		}
		if (*ptr++ == '\'' && !in_quote)
		{
			if (!in_s_quote)
				in_s_quote = TRUE;
			else
				in_s_quote = FALSE;
		}
	}
	return (in_s_quote || in_quote);
}

void	parse(t_data *data)
{
	// char	*ptr;
	// t_cmds	*node;

	// data->splited_line = ft_split(data->line, ' ');
	// if (!data->splited_line)
	// 	return (perror("Malloc"), error(data, 1, 0));
	if (check_syntax(data))
		return;
	if (check_quote(data))
		return (error(data, 'q', 0));
	expend(data);
	// ptr = data->line;
	// while (*ptr)
	// {
	// 	while (*ptr == ' ')
	// 		ptr++;
	// 	if (*ptr)
	// 	{
	// 		node = ft_calloc(1, sizeof(t_cmds));
	// 		node = ft_lstadd_back((t_list)&data->head, (t_list)node);			
	// 	}
	// }
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	(void)argc;
	(void)argv;
	ft_bzero((char *)&data, sizeof(t_data));
	data.pid = fork();
	if (!data.pid--)
		return (0);
	data.env = env;
	write(1, CLEAR, 11);
	while (1)
	{
		data.line = readline("mimishell: ");
		if (!data.line)
			break ;
		if (*data.line)
			add_history(data.line);
		parse(&data);
		ft_printf("%s\n", data.line);
		close_free_exit(&data, 0);
	}
	return (0);
}

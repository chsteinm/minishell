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

void	free_cmds_list(t_list **head)
{
	t_list	*prev;
	t_list	*node;

	node = *head;
	while (node)
	{
		ft_printf("cmd : ");
		ft_printstrs(node->cmd);
		ft_printf("file_in = %s\n", node->file_in);
		ft_printf("file_out = %s\n", node->file_out);
		ft_printf("append = %d\n", node->append_out);
		ft_printf("lim = %s\n\n", node->lim);
		ft_free_and_null(&node->file_in);
		ft_free_and_null(&node->file_out);
		ft_free_and_null(&node->cmd);
		ft_free_and_null(&node->lim);
		ft_free_and_null(&node->pipe);
		prev = node;
		node = node->next;
		ft_free_and_null(&prev);
	}
	*head = 0;
}

void	close_free_exit(t_data *data, int ret)
{
	// if (ret)
	// 	close_fds(data);
	ft_free_strings(data->splited_line);
	data->splited_line = NULL;
	free_cmds_list(&data->cmds);
	ft_lstclear(&data->cmd_param, &free);
	ft_free_and_null(&data->line);
	ft_free_and_null(&data->no_space_line);
	ft_free_and_null(&data->no_w_space_line);
	if (ret != EXIT_SUCCESS)
		exit(ret);
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

void	expand(t_data *data)
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
				return (expand(data));
			}
			else if (ft_iswhitespace(ptr[i + 1]) || ft_ismeta(ptr[i + 1]))
				continue;
			else if (ptr[i + 1] != '"' && ptr[i + 1] != '\'')
			{
				replace_var(data, ptr + i);
				return (expand(data));
			}
		}
	}
}

void	replace_white_space(char *line)
{
	while (*line)
	{
		if (ft_iswhitespace(*line))
			*line = ' ';
		line++;
	}
}

int	check_space(t_data *data)
{
	size_t	j;

	data->no_w_space_line = ft_strdup(data->line);
	if (!data->no_w_space_line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE), 1);
	replace_white_space(data->no_w_space_line);
	data->splited_line = ft_split(data->no_w_space_line, ' ');
	j = 0;
	while (data->splited_line[++j])
		if (ft_ismeta(data->splited_line[j][ft_strlen(data->splited_line[j]) - 1]))
			if (ft_ismeta(data->splited_line[j-1][ft_strlen(data->splited_line[j - 1]) - 1]))
				if (data->splited_line[j-1][ft_strlen(data->splited_line[j]) - 1] != '|')
					return (error(data, 2, data->splited_line[j][0]), 1);
	j = 0;
	while (data->splited_line[++j])
		if (ft_ismeta(data->splited_line[j][0]))
			if (ft_ismeta(data->splited_line[j-1][ft_strlen(data->splited_line[j-1]) - 1]))
				if (data->splited_line[j-1][ft_strlen(data->splited_line[j-1]) - 1] != '|')
					return (error(data, 2, data->splited_line[j][0]), 1);
	return (0);
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
			else if (*ptr != ptr[1] && *ptr != '|' && \
			(ft_ismeta(ptr[1]) || ptr[1] == '\0'))
				return (error(data, 2, ptr[1]), 1);
			else if (*ptr == ptr[1] && (ft_ismeta(ptr[2]) || ptr[1] == '\0'))
				return (error(data, 2, ptr[2]), 1);
		}
		ptr++;
	}
	// return (0);
	return (check_space(data));
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

char	*str__dup(t_data *data, char **ptr)
{
	char	quote;
	size_t	len;
	char	*str;

	len = 0;
	quote = 0;
	if (**ptr == '"' || **ptr == '\'')
	{
		quote = **ptr;
		*ptr = (*ptr) + 1;
		while ((*ptr)[len] != quote)
			len++;
	}
	else
		while ((*ptr)[len] && \
		!ft_iswhitespace((*ptr)[len]) && !ft_ismeta((*ptr)[len]))
			len++;
	str = ft_strndup(*ptr, len);
	if (!str)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE), NULL);
	if (quote)
		*ptr = (*ptr) + 1;
	*ptr = (*ptr) + len;
	return (str);
}

char	**fill_cmd(t_data *data, char **begin)
{
	char	*ptr;
	char	**cmd;
	t_list	*node_ptr;
	int		j;

	ptr = *begin;
	while (*ptr && !ft_ismeta(*ptr))
	{
		while (ft_iswhitespace(*ptr))
			ptr++;
		ft_lstadd_back(&(data->cmd_param), ft_lstnew(str__dup(data, &ptr)));
	}
	*begin = ptr;
	cmd = malloc(sizeof(char *) * (ft_lstsize(data->cmd_param) + 1));
	if (!cmd)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE), NULL);
	node_ptr = data->cmd_param;
	j = -1;
	while (node_ptr)
	{
		cmd[++j] = node_ptr->content;
		node_ptr = node_ptr->next;
	}
	cmd[++j] = 0;
	return (cmd);
}

void	parse_cmd(t_data *data, t_list *node, char **begin)
{
	char	*ptr;

	ptr = *begin;
	while (*ptr && *ptr != '|')
	{
		if (*ptr == '>')
		{
			ptr++;
			if (*ptr == '>')
			{
				node->append_out = TRUE;
				ptr++;
			}
			else
				node->append_out = FALSE;
			while (ft_iswhitespace(*ptr))
				ptr++;
			free(node->file_out);
			node->file_out = str__dup(data, &ptr);
		}
		else if (*ptr == '<')
		{
			ptr++;
			if (*ptr == '<')
			{
				ptr++;
				while (ft_iswhitespace(*ptr))
					ptr++;
				free(node->lim);
				node->lim = str__dup(data, &ptr);
				continue;
			}
			while (ft_iswhitespace(*ptr))
				ptr++;
			free(node->file_in);
			node->file_in = str__dup(data, &ptr);
		}
		else if (!ft_iswhitespace(*ptr))
			node->cmd = fill_cmd(data, &ptr);
		while (ft_iswhitespace(*ptr))
			ptr++;
	}
	ft_printf("end %c\n", *ptr);
	// if (*ptr == '|')
	// 	ptr++;
	*begin = ptr;
}

void	parse(t_data *data)
{
	char	*ptr;
	t_list	*node;

	if (check_syntax(data))
		return;
	if (check_quote(data))
		return (error(data, 'q', 0));
	expand(data);
	ptr = data->line;
	while (*ptr)
	{
		ft_printf("beg %c\n", *ptr);
		while (ft_iswhitespace(*ptr))
			ptr++;
		if (*ptr)
		{
			node = ft_calloc(1, sizeof(t_list));
			ft_lstadd_back(&data->cmds, node);			
			parse_cmd(data, node, &ptr);
		}
		if (*ptr)
			ptr++;
	}
}

int	main(int argc, char **line, char **env)
{
	t_data	data;
	(void)argc;
	(void)line;
	ft_bzero((char *)&data, sizeof(t_data));
	data.pid = fork();
	if (!data.pid--)
		return (0);
	data.env = env;
	// write(1, CLEAR, 11);
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

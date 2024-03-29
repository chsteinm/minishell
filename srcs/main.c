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
	if (ret != EXIT_SUCCESS)
		exit(ret);
}

void	error(t_data *data, int error, char c)
{
	if (error == 2 && c)
		ft_dprintf(2, ERR_SYNTX, c);
	else if (error == 2 && !c)
		ft_dprintf(2, ERR_SYNTX_NL);
	close_free_exit(data, error);
}

int		is_in_s_quote(char *line, char *ptr)
{
	int s_quote_left;
	int s_quote_right;

	s_quote_left = 0;
	s_quote_right = 0;
	while (line != ptr)
		if (*line++ == '\'')
			s_quote_left++;
	while (*ptr && *ptr != '|')
		if (*ptr++ == '\'')
			s_quote_right++;
	return (s_quote_left % 2 && s_quote_right % 2);
}

void	expend(t_data *data)
{
	char	*ptr;
	size_t	i;

	ptr = data->line;
	i = -1;
	while (ptr[++i])
		if (ptr[i] == '$' && !is_in_s_quote(ptr, ptr + i))
			ft_printf("to expend\n");
}

void	check_syntax(t_data *data)
{
	// size_t	i;
	char *ptr;

	// i = -1;
	ptr = data->line;
	while (*ptr)
	{
		if (ft_ismeta(*ptr))
		{
			if (*ptr == '|' && ptr[1] == '|')
				return (error(data, 2, *ptr));
			if (*ptr != ptr[1] && (ft_ismeta(ptr[1]) || ptr[1] == '\0'))
				return (error(data, 2, ptr[1]));
			if (*ptr == ptr[1] && (ft_ismeta(ptr[2]) || ptr[1] == '\0'))
				return (error(data, 2, ptr[2]));
		}
		ptr++;
	}
}

void	parse(t_data *data)
{
	// char	*ptr;
	// t_cmds	*node;

	// data->splited_line = ft_split(data->line, ' ');
	// if (!data->splited_line)
	// 	return (perror("Malloc"), error(data, 1, 0));
	check_syntax(data);
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
	data.env = env;
	write(1, CLEAR, 11);
	while (1)
	{
		data.line = readline("mimishell : ");
		if (!data.line)
			break ;
		if (*data.line)
			add_history(data.line);
		parse(&data);
		close_free_exit(&data, 0);
	}
	close_free_exit(&data, 0);
	return (0);
}

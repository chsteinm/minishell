#include "../includes/minishell.h"

void	error_here_doc(t_data *data, t_list *node)
{
	ft_dprintf(2, "warning: here-document at line %d", data->nb_line_hd);
	ft_dprintf(2, "delimited by end-of-file (wanted `%s')\n", node->lim);
}

void	write_until_lim(t_data *data, t_list *node)
{
	char	*line;

	while (1)
	{
		data->nb_line_hd++;
		line = readline("> ");
		if (!line)
			return (error_here_doc(data, node));
		if (!ft_strncmp(node->lim, line, ft_strlen(line) + 1))
			return (free(line));
		expand(data, &line);
		if (!ft_strncmp(node->lim, line, ft_strlen(line) + 1))
			return (free(line));
		data->tmp = line;
		line = ft_strjoin(line, "\n");
		free(data->tmp);
		if (!line)
			return (perror("malloc"), close_free_exit(data, FAILURE));
		if (write(node->pipe_heredoc[1], line, ft_strlen(line)) == -1)
		{
			perror("write");
			close_free_exit(data, FAILURE);
		}
		free(line);
	}
}

void	here_doc_manage(t_data *data, t_list *node)
{
	if (!node->fds_pipe_hd_to_close)
	{
		if (pipe(node->pipe_heredoc) == -1)
			return (perror("pipe"), close_free_exit(data, FAILURE));
		else
			node->fds_pipe_hd_to_close = TRUE;
	}
	write_until_lim(data, node);
}

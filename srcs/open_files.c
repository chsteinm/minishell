#include "../includes/minishell.h"

void	error_open(t_data *data, char *file, char **ptr)
{
	data->last_status = 1;
	perror(file);
	while (**ptr && **ptr != '|')
		(*ptr)++;
}

void	open_out(t_data *data, t_list *node, char **ptr)
{
	if (node->fd_out_to_close)
	{
		close(node->fd_out);
		node->fd_out_to_close = FALSE;
	}
	if (node->append_out)
		node->fd_out = open(node->file_out, \
		O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		node->fd_out = open(node->file_out, \
		O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (node->fd_out == -1)
		error_open(data, node->file_out, ptr);
	else
		node->fd_out_to_close = TRUE;
}

void	open_in(t_data *data, t_list *node, char **ptr)
{
	if (node->fd_in_to_close)
	{
		close(node->fd_in);
		node->fd_in_to_close = FALSE;
	}
	node->fd_in = open(node->file_in, O_RDONLY);
	if (node->fd_in == -1)
		error_open(data, node->file_in, ptr);
	else
		node->fd_in_to_close = TRUE;
}

void	write_until_lim(t_data *data, t_list *node)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line)
		{
			if (!ft_strncmp(node->lim, line, ft_strlen(line) + 1))
				return (free(line));
			expand(data, &line);
			printf("%s\n", line);
			if (!ft_strncmp(node->lim, line, ft_strlen(line) + 1))
				return (free(line));
			if (write(node->pipe_heredoc[1], line, ft_strlen(line)) == -1)
			{
				perror("write");
				close_free_exit(data, EXIT_FAILURE);
			}
			free(line);
		}
	}
}

void	here_doc_manage(t_data *data, t_list *node)
{
	if (!node->pipe_heredoc)
	{
		node->pipe_heredoc = malloc(sizeof(int) * 2);
		if (!node->pipe_heredoc)
			return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
		if (pipe(node->pipe_heredoc) == -1)
			return (perror("pipe"), close_free_exit(data, EXIT_FAILURE));
	}
	write_until_lim(data, node);
}

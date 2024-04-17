#include "../includes/minishell.h"

void	error_open(t_data *data, char *file, char **ptr)
{
	data->last_status = 1;
	perror(file);
	// ft_dprintf(2, "here");
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

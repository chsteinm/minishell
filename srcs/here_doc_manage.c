/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_manage.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:30:25 by chrstein          #+#    #+#             */
/*   Updated: 2024/06/05 12:55:07 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_line(void);

void	error_here_doc(t_data *data, t_list *node)
{
	if (g_signal == CTRL_C)
	{
		open("/dev/tty", O_RDONLY);
		data->cancel = true;
		return ;
		// return (ft_free_and_null(&node->lim));
	}
	ft_dprintf(2, "\nwarning: here-document at line %d ", data->nb_line_hd);
	ft_dprintf(2, "delimited by end-of-file (wanted `%s')\n", node->lim);
}

void	write_until_lim(t_data *data, t_list *node)
{
	char	*line;

	while (1)
	{
		data->nb_line_hd++;
		line = get_line();
		if (!line)
			return (error_here_doc(data, node));
		line[ft_strlen(line) - 1] = '\0';
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
			close_free_exit(data, FAILURE);
		free(line);
	}
}

static char	*get_line(void)
{
	ft_dprintf(1, "> ");
	return (get_next_line(0));
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
	heredoc_handle_signal(data);
	write_until_lim(data, node);
	handle_signal(data);
}
